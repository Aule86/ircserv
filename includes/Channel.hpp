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
		std::set<int> _invited;

		//MODES
		bool _hasKey;
		std::string _key;

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

		void addInvited(Client* client);
		bool isInvited(Client* client) const;
		void removeInvited(Client* client);

		bool hasKey() const;
		const std::string &getKey() const;
		void setKey(const std::string &key);
		void removeKey();
};

#endif