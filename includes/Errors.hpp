#pragma once
#include <string>

#define ERR_NOTONCHANNEL "401"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_NOTEXTTOSEND "412"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOSUCHNICK "442"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_NORECIPIENT "411" //cuando intentas enviar un msg sin dest
#define ERR_NOTEXTTOSEND "412"
//cuando intentas enviar un mensaje a un canal que no puedes enviar mensajes porque esta en ej:modo +n.
#define ERR_CANNOTSENDTOCHAN "404" 