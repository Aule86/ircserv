#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <set>

class Client;
class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::map<int, Client*> _clients; // fd -> Client*
		std::set<int> _operators;
		bool _inviteOnly;
		bool _topicRestricted;
		std::set<int> _invited;

		bool _keyActive;
		
		// Límite de usuarios
		bool _hasUserLimit;    // ¿Tiene límite activado?
		size_t _userLimit;     // ¿Cuál es el límite? 

	public:
		Channel(const std::string& name);
		~Channel();

		const std::string& getName() const;

		void addClient(Client* client);
		void removeClient(Client* client);

		bool hasClient(Client* client) const;

		void addOperator(Client* client);
		bool isOperator(Client* client) const;

		void broadcast(const std::string& msg, Client* except = NULL);
		const std::map<int, Client*>& getClients() const;

		const std::string& getTopic() const;
		void setTopic(const std::string& topic);

		bool isInviteOnly() const;
		void setInviteOnly(bool value);

		bool isTopicRestricted() const;
        void setTopicRestricted(bool value);
		bool isKeyActive() const;
		void setKeyActive(bool value);

		void addInvited(Client* client);
		bool isInvited(Client* client) const;
		void removeInvited(Client* client);
		
		// Límite de usuarios
		bool hasUserLimit() const;           // ¿Tiene límite activado?
		size_t getUserLimit() const;         // ¿Cuál es el límite?
		void setUserLimit(size_t limit);     // Establecer límite (activa el modo +l)
		void removeUserLimit();              // Quitar límite (desactiva el modo +l)
};

#endif