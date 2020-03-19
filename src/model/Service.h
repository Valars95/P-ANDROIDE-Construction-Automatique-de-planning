/*
 * Service.h
 *
 *  Created on: 8 f�vr. 2020
 *      Author: Valars
 */

#include <string>
#include <vector>
#include <map>
#include <array>
#include "Post.h"
#include "constraint/Constraint.h"


using namespace std;

#ifndef MODEL_SERVICE_H_
#define MODEL_SERVICE_H_

class Agent;

class Service {
public:
	Service();
	Service(string id);
	virtual ~Service();
	const string& getId() const;
	void setId(const string &id);

	const vector<Post*>& getPosts() const;
	void setPosts(const vector<Post*> &posts);
	void addPost(Post* post);

	const vector<Agent*>& getReferents() const;
	void setReferents(const vector<Agent*> &referents);
	void addReferent(Agent* referent);

	const vector<Constraint*>& getConstraints() const;
	void setConstraints(const vector<Constraint*> &constraints);
	void addConstraint(Constraint* constraint);

	const array<map<Post*, int>, 7>& getPostRequired() const;
	void setPostRequired(const array<map<Post*, int>, 7> &postRequired);
	void addPostRequired(Post* post, int nb); //Ajoute pour tout les jours
	void addPostRequired(Post* post, int nb, int day);

	const vector<Agent*>& getAgents() const;
	void setAgents(const vector<Agent*> &agents);
	void addAgent(Agent* agent);

protected:
	string id;
	vector<Post*> posts;
	vector<Agent*> referents; //Les r�f�rents ne sont pas forc�ment affect�s � ce service
	vector<Agent*> agents; //Agents affect�s au service
	array<map<Post*,int>, 7> postRequired; //tableau de 7 map, chaque map correspond a des couples Poste/nbRequis (C5)
	vector<Constraint*> constraints;
};

#endif /* MODEL_SERVICE_H_ */
