#include "Configuration/Config.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include <time.h>

using namespace Acore::ChatCommands;

enum WeekendXP
{
    SETTING_WEEKEND_XP_RATE = 0,

    LANG_CMD_WEEKEND_XP_SET   = 11120,
    LANG_CMD_WEEKEND_XP_ERROR = 11121,

    WD_FRIDAY   = 5,
    WD_SATURDAY = 6,
    WD_SUNDAY   = 0
};

class weekendxp_commandscript : public CommandScript
{
public:
    weekendxp_commandscript() : CommandScript("weekendxp_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "weekendxp rate", HandleSetXPBonusCommand, SEC_PLAYER, Console::No },
        };

        return commandTable;
    }

    static bool HandleSetXPBonusCommand(ChatHandler* handler, int8 rate)
    {
        Player* player = handler->GetPlayer();

        int8 maxRate = sConfigMgr->GetOption<int8>("XPWeekend.MaxAllowedRate", 2);

        if (!rate || rate > maxRate)
        {
            handler->PSendSysMessage(LANG_CMD_WEEKEND_XP_ERROR, maxRate);
            handler->SetSentErrorMessage(true);
            return true;
        }

        player->UpdatePlayerSetting("mod-double-xp-weekend", SETTING_WEEKEND_XP_RATE, rate);
        handler->PSendSysMessage(LANG_CMD_WEEKEND_XP_SET, rate);

        return true;
    }
};

class DoubleXpWeekend : public PlayerScript
{
public:
    DoubleXpWeekend() : PlayerScript("DoubleXpWeekend") { }

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("XPWeekend.Announce", false))
        {
            if (IsEventActive())
            {
                ChatHandler(player->GetSession()).PSendSysMessage("It's the Weekend! Your XP rate has been set to: %u", GetExperienceRate(player));
            }
            else
            {
                ChatHandler(player->GetSession()).PSendSysMessage("This server is running the |cff4CFF00Double XP Weekend |rmodule.");
            }
        }
    }
           

    void OnGiveXP(Player* player, uint32& amount, Unit* victim) override
    {
        if (IsEventActive())
        {
            if (sConfigMgr->GetOption<bool>("XPWeekend.QuestOnly", false) && victim)
            {
                return;
            }

            if (player->getLevel() >= sConfigMgr->GetOption<uint32>("XPWeekend.MaxLevel", 80))
            {
                return;
            }

            amount *= GetExperienceRate(player);
        }
    }

    int8 GetExperienceRate(Player * player) const
    {
        int8 rate = sConfigMgr->GetOption<int8>("XPWeekend.xpAmount", 2);

        int8 individualRate = player->GetPlayerSetting("mod-double-xp-weekend", SETTING_WEEKEND_XP_RATE).value;

        // If individualxp setting is enabled... and a rate was set, overwrite it.
        if (sConfigMgr->GetOption<bool>("XPWeekend.IndividualXPEnabled", false) && individualRate)
        {
            rate = individualRate;
        }

        // Prevent returning 0% rate.
        return rate ? rate : 1;
    }

    bool IsEventActive() const
    {
        if (sConfigMgr->GetOption<bool>("XPWeekend.AlwaysEnabled", false))
            return true;
            
        if (!sConfigMgr->GetOption<bool>("XPWeekend.Enabled", false))
            return false;

        time_t t = time(nullptr);
        tm* now = localtime(&t);

        return now->tm_wday == WD_FRIDAY || now->tm_wday == WD_SATURDAY || now->tm_wday == WD_SUNDAY;
    }
};

void AdddoublexpScripts()
{
    new DoubleXpWeekend();
    new weekendxp_commandscript();
}
