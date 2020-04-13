/*
 * Model.cpp
 *
 *  Created on: 1 mars 2020
 *      Author: Valars
 */

#include "Model.h"

using namespace std;

Model::Model(Day firstDay, int nbDays, float overtime) {
	this->firstDay = firstDay;
	this->nbDays = nbDays;
	this->overtime = overtime;
	defaultPost = NULL;
}

Model::Model(const Model& obj) {
	firstDay = obj.firstDay;
	nbDays = obj.nbDays;
	overtime = obj.overtime;
	services = obj.services;
	posts = obj.posts;
	defaultPost = obj.defaultPost;
	agents = map < Service*, std::vector<Agent*>>();

	for (auto a : obj.agents) {
		for (auto b : a.second) {
			agents[a.first].push_back(new Agent(*b));
		}
	}
}

Model::~Model() {
	for (auto a : agents)
		for (auto b : a.second) {
			delete b;
		}
	agents.clear();
}

Model& Model::operator=(const Model& obj)
{
	if (this != NULL) {
		firstDay = obj.firstDay;
		nbDays = obj.nbDays;
		overtime = obj.overtime;
		services = obj.services;
		posts = obj.posts;
		defaultPost = obj.defaultPost;
		agents = map < Service*, std::vector<Agent*>>();

		for (auto a : obj.agents) {
			for (auto b : a.second) {
				agents[a.first].push_back(new Agent(*b));
			}
		}
	}
	return Model(obj);
}


std::vector<Service*>& Model::getServices() {
	return services;
}

void Model::addAgent(Agent* agent, Service* service) {
	agents[service].push_back(agent);
	service->addAgent(agent);
}

void Model::addService(Service* service) {
	services.push_back(service);
}

void Model::printPlanning() {

	cout << "Jours:\t";
	for (int i = 0; i < nbDays; i++) {
		cout << "\t" << i + 1;
	}
	cout << endl;
	for (auto s : agents)
	{
		for (auto agent : s.second)
		{
			cout << "Agent " << agent->getId() << ":\t";

			for (unsigned int j = 0; j < agent->getCalendar().size(); j++)
			{
				if (agent->getCalendar()[j] != NULL)
				{
					cout << agent->getCalendar()[j]->getId();
				}
				else
					cout << "null";
				cout << "\t";
			}
			cout << endl;
		}


	}
}

Day Model::getFirstDay() const {
	return firstDay;
}

void Model::setFirstDay(Day firstDay) {
	this->firstDay = firstDay;
}

int Model::getNbDays() const {
	return nbDays;
}

void Model::setNbDays(int nbDays) {
	this->nbDays = nbDays;
}

float Model::getOvertime() const {
	return overtime;
}

std::vector<Agent*> Model::getAgentFrom(Service* service) {
	return agents[service];
}

void Model::setOvertime(float overtime) {
	this->overtime = overtime;
}

Day Model::getNextDay(Day day) {
	switch (day) {
	case Monday:
		return Tuesday;
	case Tuesday:
		return Wednesday;
	case Wednesday:
		return Thursday;
	case Thursday:
		return Friday;
	case Friday:
		return Saturday;
	case Saturday:
		return Sunday;
	case Sunday:
		return Monday;
	default:
		return None;
	}
}

Post* Model::getDefaultPost() {
	return defaultPost;
}

void Model::setDefaultPost(Post* defaultPost) {
	this->defaultPost = defaultPost;
}

Model Model::generateModelInstance(Day firstDay, int nbDays, float overtime, int nbServices, int nbPosts, int nbAgents, float nbHoursWeek, int nbAgentsPerService, int nbPostsPerService) {
	/*
	Si nbPostsPerService est indiqu�, le nombre nbPosts ne sera pas respect� si nbPosts < nbPostsPerService*nbServices
	de m�me pour nbAgentsPerService
	*/

	Model m = Model(firstDay, nbDays, overtime);
	m.defaultPost = NULL;

	int nbPostsAvailable = nbPosts, nbPostsService_i = -1, testLengthPost = -1;

	//Services et Postes

	//poste cong�/repos dispos dans chaque service
	Post* repos = new Post("Repos", 0.0);
	repos->addAttribut("rest");

	Post* ca = new Post("Ca", 0.0);
	ca->addAttribut("rest");
	ca->addAttribut("ca");

	m.setDefaultPost(repos);
	m.setDefaultPost(ca);

	cout << nbPostsPerService << endl;
	if (nbPostsPerService > -1) {
		nbPostsService_i = nbPostsPerService;
	}

	//attribution de postes pour chaque service
	for (int i = 0; i < nbServices; i++) {
		Service* service_i = new Service(std::to_string(i));
		m.addService(service_i);

		service_i->addPost(repos); //ajout des postes obligatoires en dur
		service_i->addPost(ca);

		if (nbPostsPerService == -1) {
			if (i != nbServices - 1 && nbPostsAvailable >= 2) {
				nbPostsService_i = 2 + rand() % (nbPostsAvailable / 2); //au moins 2 postes par service (nbPosts est donc approximatif)
			}
			else { //si c'est le dernier service : lui attribuer tous les posts restants
				nbPostsService_i = nbPostsAvailable; //solution temporaire : risque d'avoir bcp + de posts dans le dernier service
			}
		}

		//pour chaque post qui sera dans ce service
		for (int j = 0; j < nbPostsService_i; j++) {
			testLengthPost = rand() % 10 + 1;
			Post* posti_j;
			if (testLengthPost >= 5) { //50% de chance de cr�er un poste � 12.25h, sinon 7.5h
				posti_j = new Post("P" + std::to_string(i) + "_" + std::to_string(j), 12.25);
			}
			else {
				posti_j = new Post("P" + std::to_string(i) + "_" + std::to_string(j), 7.5);
			}
			//i_j->addAttribut("workL");			//comment faire ?
			service_i->addPost(posti_j);
			//ghr->addPostRequired(posti_j, 1);     //idem
		}

		service_i->addPost(repos); //chaque service a un poste Repos

		nbPostsAvailable -= nbPostsService_i;
	}

	//ajouter contraintes ?


	//Agents

	//variables pour les tirages al�atoires
	int service_rand = 0, post_rand = 0, conges_rand = 0, jour_rand = 0, incr_jour = 0;

	//pour chaque agents
	for (int i = 0; i < nbAgents; i++) {
		int dice = rand() % 10 + 1;
		Agent* a_i;
		if (dice <= 2) { //20% de chance d'avoir un agent qui d�bute
			a_i = new Agent(std::to_string(i), i, nbHoursWeek, Status::Beginner);
		}
		else {
			a_i = new Agent(std::to_string(i), i, nbHoursWeek, Status::Confirmed);
		}

		//si aucun nombre pr�cis d'agents par service n'est donn�, random pour attribuer un service
		if (nbAgentsPerService == -1) {
			service_rand = rand() % nbServices;
			a_i->setService(m.getServices()[service_rand]);
		}
		// sinon, ajout de l'agent au premier service libre
		else {
			int j = 0;
			while (m.getServices()[j]->getAgents().size() >= nbAgentsPerService) {
				j++;
			}
			a_i->setService(m.getServices()[j]);
		}

		for (int jour = 0; jour < nbDays; jour++) {
			conges_rand = rand() % 100 + 1;
			//cout << i << ": " << conges_rand << endl;
			if (conges_rand <= 5) { //5% de chance d'avoir un premier cong� dans le mois
				jour_rand = rand() % nbDays + 1; //random pour choisir le jour du cong�

				a_i->setCalendarDay(ca, jour_rand, true);

				//chances de faire une suite de cong�s apr�s un premier cong� = 55%
				conges_rand = rand() % 100 + 1;
				//cout << i << " suite de cong�s : " << conges_rand << endl;
				while (conges_rand <= 55 && jour_rand + incr_jour <= nbDays) { //ne pas d�passer le dernier jour du mois (pas forc�ment r�aliste)
					a_i->setCalendarDay(ca, jour_rand + incr_jour, true);
					conges_rand = rand() % 10 + 1;
					incr_jour++;
				}
				incr_jour = 0;
			}
		}
		m.addAgent(a_i, m.getServices()[service_rand]);
	}

	return m;
}