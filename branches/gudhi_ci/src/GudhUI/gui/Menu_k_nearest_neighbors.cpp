/*
 * Menu_k_nearest_neighbors.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: dsalinas
 */


#include "Menu_k_nearest_neighbors.h"

Menu_k_nearest_neighbors::Menu_k_nearest_neighbors(QMainWindow* parent_):
parent(parent_)
{
	setupUi(this);
	connectActions(parent_);
}

void Menu_k_nearest_neighbors::connectActions(QMainWindow* parent){

	QObject::connect(
			this->pushButtonCompute,
			SIGNAL(clicked()),
			this,
			SLOT(send_compute_k_nearest_neighbors())
	);
	QObject::connect(
			this->spinBoxK,
			SIGNAL(valueChanged(int)),
			this,
			SLOT(update_k(int))
	);
	QObject::connect(
			this,
			SIGNAL(compute_k_nearest_neighbors(unsigned)),
			parent,
			SLOT(build_k_nearest_neighbors(unsigned))
	);

}

void Menu_k_nearest_neighbors::send_compute_k_nearest_neighbors(){
	emit(compute_k_nearest_neighbors((unsigned)spinBoxK->value()));
}

void Menu_k_nearest_neighbors::accept(){
	send_compute_k_nearest_neighbors();
}

void Menu_k_nearest_neighbors::update_k(int new_k_value){
	if(checkBoxAutoUpdate->isChecked())
		emit(compute_k_nearest_neighbors((unsigned)spinBoxK->value()));
}


#include "Menu_k_nearest_neighbors.moc"