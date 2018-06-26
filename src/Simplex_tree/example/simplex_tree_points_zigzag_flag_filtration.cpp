#include <iostream>
#include <fstream>
#include <chrono>
#include <gudhi/Simplex_tree.h>
#include "gudhi/reader_utils.h"
#include <gudhi/distance_functions.h>
#include <gudhi/Zigzag_filtration.h>
#include <gudhi/choose_n_farthest_points.h>
#include <gudhi/Points_off_io.h>
#include <boost/program_options.hpp>
#include <CGAL/Epick_d.h>
#include <gudhi/choose_n_farthest_points.h>
#include <gudhi/pick_n_random_points.h>


// Types definition
using Simplex_tree = Gudhi::Simplex_tree<Gudhi::Simplex_tree_options_zigzag_persistence>;
using Zz_edge = Zigzag_edge<Simplex_tree>;
using Filtration_value = Simplex_tree::Filtration_value;
// using Zigzag_persistence = Gudhi::zigzag_persistence::Zigzag_persistence<Simplex_tree >;
// using Point = std::vector<double>;
using K = CGAL::Epick_d<CGAL::Dynamic_dimension_tag>;
using Point_d = typename K::Point_d;
using Points_off_reader = Gudhi::Points_off_reader<Point_d>;

void program_options( int argc, char* argv[]
                    , std::string& off_file_points
                    , Filtration_value& nu
                    , Filtration_value& mu
                    , int& dim_max);

int main(int argc, char* argv[])
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  int enlapsed_sec;

  std::string off_file_points;
  Filtration_value nu, mu;
  int dim_max;

  program_options(argc, argv, off_file_points, nu, mu, dim_max);

  std::vector< Zz_edge >        edge_filtration;
  std::vector<Filtration_value> filtration_values;
  //extract points from file
  Points_off_reader off_reader(off_file_points); //read points

  //sort points
  start = std::chrono::system_clock::now();
  std::vector<Point_d> sorted_points;
  Gudhi::subsampling::choose_n_farthest_points( K(), off_reader.get_point_cloud() 
    , off_reader.get_point_cloud().size() //all points
    , 0//start with point [0]//Gudhi::subsampling::random_starting_point
    , std::back_inserter(sorted_points));
  end = std::chrono::system_clock::now();
  enlapsed_sec =std::chrono::duration_cast<std::chrono::seconds>(end-start).count();

  std::cout << "Furthest point sort: " << enlapsed_sec << " sec.\n";

  //Compute edge filtration
  start = std::chrono::system_clock::now();
	fast_points_to_edge_filtration( sorted_points, 
                             Gudhi::Euclidean_distance(),
                             nu, mu, filtration_values, edge_filtration );
  end = std::chrono::system_clock::now();
  enlapsed_sec =std::chrono::duration_cast<std::chrono::seconds>(end-start).count();

  std::cout << "Edge filtration computation (fast): " << enlapsed_sec << " sec.\n";
  

  //second opinion 
  std::vector< Zz_edge >        edge_filtration_2;
  std::vector<Filtration_value> filtration_values_2;

  start = std::chrono::system_clock::now();
  points_to_edge_filtration( sorted_points, 
                             Gudhi::Euclidean_distance(),
                             nu, mu, filtration_values_2, edge_filtration_2 );
  end = std::chrono::system_clock::now();
  enlapsed_sec =std::chrono::duration_cast<std::chrono::seconds>(end-start).count();

  std::cout << "Edge filtration computation (slow): " << enlapsed_sec << " sec.\n";



  std::cout << (filtration_values == filtration_values_2) << "\n";
  std::cout << (edge_filtration == edge_filtration_2) << "\n";


//sort points
  // typedef CGAL::Epick_d< CGAL::Dynamic_dimension_tag > K;
  // typedef typename K::Point_d Point_d;

  // K k;
  // std::vector<Point_d> results;
  // Gudhi::subsampling::choose_n_farthest_points(
  //     k //kernel
  //   , input_pts //input points
  //   , final_size //final size
  //   , 0 //starting point 
  //   , back_inserter(results) );

  std::cout << "Point cloud : \n";
  for(auto point : off_reader.get_point_cloud()) {
    for(auto x : point) { std::cout << x << " "; }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Epsilon filtration values: \n";
  for(size_t i = 0; i < filtration_values.size(); ++i) {
    std::cout << "eps_" << i << " : " << filtration_values[i] << 
    "  --  " << filtration_values_2[i] << 
    std::endl;
  }
  std::cout << std::endl;

  std::cout << "Edge filtration: \n";
  size_t i=0;
  for(auto edg : edge_filtration) 
  { 
   if(edg.type()) { std::cout << "+ "; } else { std::cout << "- "; }
    std::cout <<  " " << edg.u() << " " << edg.v() << "  " << edg.fil() << 
    "  --  ";
    if(edge_filtration_2[i].type()) { std::cout << "+ "; } else { std::cout << "- "; }
    std::cout << " " << edge_filtration_2[i].u() << " " << edge_filtration_2[i].v() << "  " << edge_filtration_2[i].fil() << 
    std::endl;
    ++i;
  }
  std::cout << std::endl;
 
  // traverse the filtration
  Simplex_tree st;
  st.initialize_filtration(edge_filtration, dim_max); 
  auto zz_rg = st.filtration_simplex_range();
  
  // auto zz_rg = st.zigzag_simplex_range(edge_filtration, dim_max);

  std::cout << "Simplex filtration: \n";
  for(auto it = zz_rg.begin(); it != zz_rg.end(); ++it ) {
    if(it.arrow_direction()) {std::cout << "+ ";} else {std::cout << "- ";}
    for(auto u : st.simplex_vertex_range(*it)) { std::cout << u << " "; }
      std::cout << "    " << st.filtration(*it) << "\n";
  }
  std::cout << std::endl;

  return 0;
}

 
//program options
void program_options(int argc, char* argv[], std::string& off_file_points, Filtration_value& nu, Filtration_value &mu, int& dim_max) {
  namespace po = boost::program_options;
  po::options_description hidden("Hidden options");
  hidden.add_options()("input-file", po::value<std::string>(&off_file_points),
                       "Name of an OFF file containing a point set.\n");

  po::options_description visible("Allowed options", 100);
  visible.add_options()("help,h", "produce help message")
  ( "nu",
    po::value<Filtration_value>(&nu)->default_value(3.0),
    "Lower multiplicative factor in the oscillating Rips zigzag filtration.")
  ( "mu",
    po::value<Filtration_value>(&mu)->default_value(3.2),
    "Upper multiplicative factor in the oscillating Rips zigzag filtration.")
  ( "cpx-dimension,d", po::value<int>(&dim_max)->default_value(1),
    "Maximal dimension of the oscillating Rips complexes in the filtration.")
  // ( "field-charac,p", po::value<int>(&p)->default_value(11),
  //   "Characteristic p of the coefficient field Z/pZ for computing homology.")
  // ( "min-persistence,m", po::value<Filtration_value>(&min_persistence),
  //   "Minimal lifetime of homology feature to be recorded. Default is 0. Enter a negative value to see zero length "
  //     "intervals")
  ;

  po::positional_options_description pos;
  pos.add("input-file", 1);

  po::options_description all;
  all.add(visible).add(hidden);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(all).positional(pos).run(), vm);
  po::notify(vm);

  if (vm.count("help") || !vm.count("input-file")) {
    std::cout << std::endl;
    std::cout << "Compute the oscillating Rips zigzag filtration based on a point cloud, with Euclidean metric.\n\n";
    std::cout << "Usage: " << argv[0] << " [options] input-file" << std::endl << std::endl;
    std::cout << visible << std::endl;
    std::abort();
  }
}