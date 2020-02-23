/*
 * ConstraintDayNeed.h
 *
 *  Created on: 21 f�vr. 2020
 *      Author: Valars
 */

#ifndef SRC_MODEL_CONSTRAINT_CONSTRAINTDAYNEED_H_
#define SRC_MODEL_CONSTRAINT_CONSTRAINTDAYNEED_H_

#include "Constraint.h"
#include "Post.h"
#include <map>

//Correspondant � la contrainte C5

class ConstraintDayNeed: public Constraint {
public:
	ConstraintDayNeed(std::map<Post,int> needs, int priority);
	virtual ~ConstraintDayNeed();
	const std::map<Post, int>& getNeeds() const;
	void setNeeds(const std::map<Post, int> &needs);
	int getPriority() const;
	void setPriority(int priority);

protected:
	int priority;
	std::map<Post,int> needs; //Cl� = post, Valeur = nbRequis dans la journ�e

};

#endif /* SRC_MODEL_CONSTRAINT_CONSTRAINTDAYNEED_H_ */
