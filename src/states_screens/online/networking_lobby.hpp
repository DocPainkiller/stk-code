//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013-2015 Glenn De Jonghe
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_NETWORKING_LOBBY_HPP
#define HEADER_NETWORKING_LOBBY_HPP

#include "guiengine/screen.hpp"
#include "guiengine/widgets/text_box_widget.hpp"
#include <map>
#include <memory>
#include <tuple>
#include <utility>

class InputDevice;
class Server;
enum KartTeam : int8_t;
struct LobbyPlayer;

namespace GUIEngine
{ 
    class ButtonWidget;
    class LabelWidget;
    class ListWidget;
    class IconButtonWidget;
    class TextBoxWidget;
}

namespace irr
{
    namespace gui
    {
        class STKModifiedSpriteBank;
    }
}

/**
  * \brief Handles the networking lobby
  * \ingroup states_screens
  */
class NetworkingLobby : public GUIEngine::Screen,
                        public GUIEngine::ScreenSingleton<NetworkingLobby>,
                        public GUIEngine::ITextBoxWidgetListener
{
private:
    enum LobbyState
    {
        LS_ADD_PLAYERS,
        LS_CONNECTING
    } m_state;

    friend class GUIEngine::ScreenSingleton<NetworkingLobby>;

    NetworkingLobby();

    uint64_t m_ping_update_timer;
    std::map<std::string, LobbyPlayer> m_player_names;
    std::shared_ptr<Server> m_joined_server;
    std::vector<core::stringw> m_server_info;
    int m_server_info_height;

    core::stringw m_start_text, m_ready_text, m_live_join_text,
        m_configuration_text, m_spectate_text;

    float m_start_timeout;
    int64_t m_cur_starting_timer;
    unsigned m_min_start_game_players;

    bool m_allow_change_team, m_has_auto_start_in_server,
        m_server_configurable, m_client_live_joinable;

    video::ITexture* m_config_texture;
    video::ITexture* m_spectate_texture;

    GUIEngine::IconButtonWidget* m_back_widget;
    GUIEngine::LabelWidget* m_header;
    GUIEngine::LabelWidget* m_text_bubble;
    GUIEngine::LabelWidget* m_timeout_message;
    GUIEngine::IconButtonWidget* m_start_button;
    GUIEngine::IconButtonWidget* m_config_button;
    GUIEngine::ListWidget* m_player_list;
    GUIEngine::TextBoxWidget* m_chat_box;
    GUIEngine::ButtonWidget* m_send_button;

    irr::gui::STKModifiedSpriteBank* m_icon_bank;

    /** \brief implement optional callback from parent class GUIEngine::Screen */
    virtual void unloaded() OVERRIDE;

    virtual void onTextUpdated() OVERRIDE {}
    virtual bool onEnterPressed(const irr::core::stringw& text) OVERRIDE
    {
        sendChat(text);
        return true;
    }

    void sendChat(irr::core::stringw text);
    void updatePlayerPings();

public:

    virtual void onUpdate(float delta) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget, const std::string& name,
                               const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void beforeAddingWidget() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void tearDown() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual bool onEscapePressed() OVERRIDE;

    void finishAddingPlayers();
    void addMoreServerInfo(core::stringw info);
    void setJoinedServer(std::shared_ptr<Server> server)
    {
        m_joined_server = server;
        m_server_info.clear();
    }
    void updatePlayers();
    void openSplitscreenDialog(InputDevice* device);
    void addSplitscreenPlayer(irr::core::stringw name);
    void cleanAddedPlayers();
    void initAutoStartTimer(bool grand_prix_started, unsigned min_players,
                            float start_timeout, unsigned server_max_player);
    void setStartingTimerTo(float t);
    void toggleServerConfigButton(bool val)    { m_server_configurable = val; }
};   // class NetworkingLobby

#endif
