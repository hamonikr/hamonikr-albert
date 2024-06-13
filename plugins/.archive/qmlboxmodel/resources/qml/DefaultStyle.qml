﻿import Qt5Compat.GraphicalEffects
import QtQml.StateMachine as DSM  // State name clashes.
import QtQuick
import QtQuick.Window
import "albert.js" as Util

Item { DebugRect{ name: "root" }
    id: root
    objectName: "root"
    property bool debug: style.debug

    width: frame.width + (style.shadow_enabled ? style.shadow_size*2 : 0)
    height: frame.height + (style.shadow_enabled ? style.shadow_size*2 + style.shadow_size/3 : 0) // style.shadow_size/2 is vertical offset

    layer.enabled: true
    layer.effect: DropShadow {
        id: shadow
        transparentBorder: true
        verticalOffset: style.shadow_size/3
        radius: style.shadow_enabled ? style.shadow_size : 0
        color: style.shadow_enabled ? style.shadow_color : "#00000000"

        // samples: Not available in below 6.4. Branch and set dynamically
        Component.onCompleted: {
            if (QT_VERSION_MAJOR === 6 && QT_VERSION_MINOR >= 4)
                shadow.samples = style.shadow_size * 2
        }
    }


    Rectangle
    {
        DebugRect{ name: "frame" }
        id: frame
        x: style.shadow_enabled ? style.shadow_size : 0
        y: style.shadow_enabled ? style.shadow_size : 0
        width: style.window_width
        height: contentColumn.height

        color: style.window_background_color
        border.color: style.window_border_color
        border.width: style.window_border_width
        radius: style.window_radius

        Column
        {
            id: contentColumn
            width: frame.width
            padding: style.window_content_padding
            spacing: style.window_content_spacing

            Rectangle
            {
                DebugRect{ name: "inputFrame" }
                id: inputFrame
                height: input.height + style.input_padding * 2
                width: contentColumn.width - style.window_content_padding * 2

                color: style.input_background_color
                border.color: style.input_border_color
                border.width: style.input_border_width
                radius: style.input_radius

                HistoryTextInput
                {
                    DebugRect{ name: "HistoryTextInput" }
                    id: input
                    objectName: "input" // required by C++
                    x:style.input_padding;
                    anchors.verticalCenter: parent.verticalCenter
                    width: Math.min(implicitWidth, parent.width - 2*style.input_padding)

                    font.pixelSize: style.input_text_font_size
                    color: style.input_text_color
                    selectedTextColor: style.input_text_selection_color
                    selectionColor: style.input_text_selection_background_color
                    cursorColor: style.input_cursor_color
                    Text
                    {
                        DebugRect{ name: "Trigger" }
                        id: trigger
                        visible: hint.width !== 0
                        font.pixelSize: style.input_text_font_size
                        color: style.input_text_trigger_color
                    }
                    onTextChanged: hint.text = ''
                    Keys.forwardTo: [resultsList]
                    //Keys.onPressed: (event)=>{ Util.printKeyPress("inputLine", event) }
                }

                Text
                {
                    DebugRect{ name: "hint" }
                    id: hint
                    anchors.left: input.right
                    anchors.right: inputFrame.right
                    anchors.verticalCenter: input.verticalCenter
                    anchors.rightMargin: style.input_padding

                    font.pixelSize: style.input_text_font_size
                    color: style.input_hint_color
                    elide: Text.ElideRight
                }

            }

            DesktopListView { DebugRect{ name: "resultsList" }
                id: resultsList
                objectName: "resultsList"
                width: contentColumn.width - contentColumn.padding*2 - style.result_list_padding*2
                anchors.horizontalCenter: parent.horizontalCenter
                maxItems: style.result_list_max_visible


                // visible: count !== 0
                spacing: style.result_list_spacing
                delegate: ResultItemDelegate { DebugRect{ name: "resultItem" }
                    icon_size: style.result_item_icon_size
                    animation_duration: style.animation_duration
                    text_color: style.result_item_text_color
                    text_color_current: style.result_item_text_color_current
                    text_font_size: style.result_item_text_font_size
                    subtext_color: style.result_item_subtext_color
                    subtext_color_current: style.result_item_subtext_color_current
                    subtext_font_size: style.result_item_subtext_font_size
                    padding: style.result_item_padding
                    spacing_horizontal: style.result_item_spacing_horizontal
                    spacing_vertical: style.result_item_spacing_vertical
                    onClicked: (result_index) => { resultsList.itemActivated(result_index) }
                }
                highlight: Rectangle { DebugRect{ name: "highlight" }
                    color: style.result_list_highlight_background_color
                    border.color: style.result_list_highlight_border_color
                    border.width: style.result_list_highlight_radius
                    radius: style.result_list_highlight_radius
                }
                onItemActivated: (result_index) => { activate(result_index) }
                onCurrentIndexChanged: if (currentIndex >= 0) setInputActionHint(currentItem.getInputAction())
                Keys.forwardTo: [root]
                //Keys.onPressed: (event)=>{ Util.printKeyPress("resultsList", event) }
            }

            DesktopListView { DebugRect{ name: "actionsList" }
                id: actionsList
                objectName: "actionsList"
                width: contentColumn.width - contentColumn.padding*2 - style.action_list_padding*2
                anchors.horizontalCenter: parent.horizontalCenter
                onCountChanged: {
                    height = (count === 0) ? 0 : count * (contentItem.children[0].height + spacing) - spacing
                }

                visible: count !== 0
                spacing: style.action_list_spacing

                delegate: ActionItemDelegate { DebugRect{ name: "actionItem" }
                    color: style.action_item_text_color
                    color_current: style.action_item_text_color_current
                    font_size: style.action_item_text_font_size
                    padding: style.action_item_padding
                    onClicked: (action_index) => { actionsList.itemActivated(action_index) }
                }
                highlight: Rectangle { DebugRect{ name: "highlight" }
                    color: style.action_list_highlight_background_color
                    border.color: style.action_list_highlight_border_color
                    border.width: style.action_list_highlight_radius
                    radius: style.action_list_highlight_radius
                }
                onItemActivated: (action_index) => { activate(resultsList.currentIndex, action_index) }
                //Keys.onPressed: (event)=>{ Util.printKeyPress("actionsList", event) }
                Keys.forwardTo: [root]
            }
        }

        SettingsButton { DebugRect{ name: "sb" }
            id: settingsButton
            anchors.top: frame.top
            anchors.right: frame.right
            anchors.topMargin: style.settings_button_margin
            anchors.rightMargin: style.settings_button_margin
            width: style.settings_button_size
            height: style.settings_button_size
            iconColor: style.settings_button_color
            acceptedButtons: Qt.LeftButton|Qt.RightButton
            onClicked: (mouse) => {
                if (mouse.button === Qt.RightButton)
                    style.debug = !style.debug
                else if (mouse.button === Qt.LeftButton)
                    albert.showSettings()
            }
        }
    }

    QtObject {
        id: style
        objectName: "style"
        property bool debug: false

        // General
        property int  animation_duration: 100
        // property font font_family: Qt.application.font  // custom fonts led to weird padding on some systems

        // Shadow
        property bool   shadow_enabled: true
        property color  shadow_color: "#60000000"
        property int    shadow_size: 60

        // Frame
        property color  window_background_color: palette.window
        property color  window_border_color: palette.light
        property int    window_border_width: 4
        property int    window_content_padding: 10
        property int    window_content_spacing: 10
        property int    window_radius: 14
        property int    window_width: 640

        // Input
        property color  input_background_color: palette.midlight
        property color  input_border_color: 'transparent'
        property color  input_cursor_color: palette.text
        property color  input_hint_color: palette.placeholderText
        property color  input_text_color: palette.text
        property color  input_text_selection_background_color: palette.highlight
        property color  input_text_selection_color: palette.highlightedText
        property color  input_text_trigger_color: palette.highlight
        property int    input_border_width: 0
        property int    input_padding: 4
        property int    input_radius: 6
        property int    input_text_font_size: 28

        // Results list
        property color result_list_highlight_background_color: palette.highlight
        property color result_list_highlight_border_color: palette.highlight
        property int   result_list_highlight_border_width: 0
        property int   result_list_highlight_radius: 6
        property int   result_list_max_visible: 5
        property int   result_list_padding: 0
        property int   result_list_spacing: 0

        // Result item
        property color result_item_subtext_color: palette.placeholderText
        property color result_item_subtext_color_current: palette.highlightedText
        property color result_item_text_color: palette.text
        property color result_item_text_color_current: palette.highlightedText
        property int   result_item_icon_size: 40
        property int   result_item_padding: 4
        property int   result_item_spacing_horizontal: 8
        property int   result_item_spacing_vertical: 0
        property int   result_item_subtext_font_size: 12
        property int   result_item_text_font_size: 20

        // Actions list
        property color action_list_highlight_background_color: palette.highlight
        property color action_list_highlight_border_color: palette.highlight
        property int   action_list_highlight_border_width: 0
        property int   action_list_highlight_radius: 6
        property int   action_list_padding: 0
        property int   action_list_spacing: 0

        // Action item
        property color action_item_text_color: palette.text
        property color action_item_text_color_current: palette.highlightedText
        property int   action_item_padding: 4
        property int   action_item_text_font_size: 14

        // Settings button
        property color settings_button_color: palette.highlight
        property int   settings_button_margin: 14
        property int   settings_button_size: 14
    }

    DSM.StateMachine {
        id: statemachine
        running: true
        initialState: sRoot

        component StateDebug: DSM.State {
            property string name
            onEntered: albert.debug("ENTER " + name)
            onExited: albert.debug("EXIT " + name)
        }

        DSM.State {
            id: sRoot
            childMode: DSM.QState.ParallelStates

            DSM.State {
                initialState: sIdle
                StateDebug { id: sIdle; name: "sIdle"
                    DSM.SignalTransition { targetState: sBusy; signal: input.textChanged }
                }
                StateDebug { id: sBusy; name: "sBusy"
                    initialState: sBusyAwaitQuery
                    onEntered: settingsButton.busy = true;
                    onExited: settingsButton.busy = false
                    DSM.SignalTransition { targetState: sIdle; signal: albert.currentQueryChanged; guard: albert.currentQuery() === null }
                    DSM.SignalTransition { targetState: sIdle; signal: albert.currentQueryFinished }

                    StateDebug { id: sBusyAwaitQuery; name: "sBusyAwaitQuery"
                        onEntered: { resultsList.enabled = false; actionsList.enabled = false; }
                        onExited: { resultsList.enabled = true; actionsList.enabled = true; }
                        DSM.SignalTransition { targetState: sBusyDisplayQuery; signal: albert.currentQueryChanged; guard: albert.currentQuery() !== null }
                    }

                    StateDebug { id: sBusyDisplayQuery; name: "sBusyDisplayQuery"
                        DSM.SignalTransition { targetState: sBusyAwaitQuery; signal: input.textChanged }
                    }
                }
            }

            DSM.State {  // Model state
                initialState: sQueryUnset

                StateDebug { id: sQueryUnset; name: "sQueryUnset"
                    onEntered: resultsList.model = null
                    DSM.SignalTransition { targetState: sQuerySet; signal: albert.currentQueryChanged; guard: albert.currentQuery() !== null }
                }

                StateDebug { id: sQuerySet; name: "sQuerySet"
                    childMode: DSM.QState.ParallelStates
                    DSM.SignalTransition { targetState: sQueryUnset; signal: albert.currentQueryChanged; guard: albert.currentQuery() === null }
                    DSM.SignalTransition { targetState: sMatches; signal: albert.currentQueryChanged; guard: albert.currentQuery() !== null }

                    DSM.State {
                        initialState: sMatches
                        onExited: resultsList.model = null

                        StateDebug { id: sMatches; name: "sMatches"
                            onEntered: {
                                albert.critical(albert.currentQuery().string() + albert.currentQuery())
                                albert.critical(albert.currentQuery().matches().rowCount())
                                resultsList.model = albert.currentQuery().matches()

                            }
                            DSM.SignalTransition { targetState: sFallbacks; signal: showFallbacks }
                        }
                        StateDebug { id: sFallbacks; name: "sFallbacks"
                            onEntered: resultsList.model = albert.currentQuery().fallbacks()
                            DSM.SignalTransition { targetState: sMatches; signal: showMatches;
                                guard: !albert.currentQuery().isFinished() || albert.currentQuery().matches().rowCount() > 0
                            }
                        }
                    }

                    // SignalTransition { targetState: sFallbacks;      signal: albert.currentQueryReady; guard: albert.currentQuery().matches().rowCount() === 0 && !albert.currentQuery().isTriggered()}
                    // SignalTransition { targetState: sDisplayMatches; signal: albert.currentQueryReady; guard: albert.currentQuery().matches().rowCount() > 0 || albert.currentQuery().isTriggered()}

                    DSM.State {
                        initialState: sItems
                        StateDebug { id: sItems; name: "sItems"
                            DSM.SignalTransition { targetState: sActions; signal: showActions; guard: resultsList.currentIndex >= 0 }
                        }
                        StateDebug { id: sActions; name: "sActions"
                            onEntered: {
                                actionsList.model = albert.createStringListModel(resultsList.currentItem.getActionsList())
                                input.Keys.forwardTo = [actionsList]
                            }
                            onExited: {
                                actionsList.model = null
                                input.Keys.forwardTo = [resultsList]
                            }
                            DSM.SignalTransition { targetState: sItems; signal: hideActions }
                            DSM.SignalTransition { targetState: sItems; signal: sMatches.entered }
                            DSM.SignalTransition { targetState: sItems; signal: sFallbacks.entered }
                            DSM.SignalTransition { targetState: sItems; signal: albert.currentQueryChanged }
                        }
                    }
                }
            }
        }
    }

    signal showActions()
    signal hideActions()
    signal showFallbacks()
    signal showMatches()

    Keys.onPressed: (event)=> {
        //Util.printKeyPress("root", event)

        if (Util.testKey(event, Qt.Key_Meta))
            showFallbacks()

        else if (Util.testKey(event, Qt.Key_Alt))
            showActions()

        else if (Util.testAnyKeyCombination(event, Qt.ControlModifier, [Qt.Key_Enter, Qt.Key_Return]))
            if (sItems.active)
                showActions()
            else
                hideActions()

        else if (sItems.active
                 && (resultsList.currentIndex < 1 && !event.isAutoRepeat && Util.testKey(event, Qt.Key_Up) // First
                     || Util.testKeyCombination(event, Qt.ShiftModifier, Qt.Key_Up)))
            input.historyNext()

        else if (sItems.active && Util.testKeyCombination(event, Qt.ShiftModifier, Qt.Key_Down))
            input.historyPrev()

        else if (Util.testKeyCombination(event, Qt.ControlModifier, Qt.Key_Comma))
            albert.showSettings()

        else if (resultsList.currentIndex >= 0 && Util.testKeyCombination(event, Qt.NoModifier, Qt.Key_Tab))
            applyInputAction(resultsList.currentItem.getInputAction())
    }

    Keys.onReleased: (event)=> {
        if (Util.testKey(event, Qt.Key_Meta))
            showMatches()
        else if (Util.testKey(event, Qt.Key_Alt))
            hideActions()
    }

    // Window.onVisibilityChanged: {
    //     Window.visibility === Window.Hidden ? albert.clearQueries() : runQuery()
    // }

    function activate(itemIndex, actionIndex) {
        console.info("activate " + itemIndex + " " + actionIndex)
        if (typeof actionIndex === 'undefined')
            actionIndex = 0

        if (sMatches.active)
            albert.currentQuery().activateMatch(itemIndex, actionIndex)
        else
            albert.currentQuery().activateFallback(itemIndex, actionIndex)
        mainWindow.hide()
    }

    // function runQuery() {
    //     let query = albert.query(input.text)
    //     trigger.text = query.trigger()
    //     hint.text = (query.string().length === 0) ? query.synopsis() : ""
    //     query.run()
    //     settingsButton.busy = true
    // }


    Connections{
        target: albert
        function onCurrentQueryChanged()
        {
            let q = albert.currentQuery()
            if (q !== null)
            {
                albert.critical("query.string() "+q.string())
                albert.critical("query.trigger() "+q.trigger())
                trigger.text = q.trigger()
                hint.text = (q.string().length === 0) ? q.synopsis() : ""
            }
            // settingsButton.busy = false
        }
    }
    Connections{
        target: albert
        function onCurrentQueryFinished()
        {
            // settingsButton.busy = false
        }
    }

    function applyInputAction(inputActionText) {
        if (inputActionText.length !== 0){
            input.text = /*albert.currentQuery().trigger() +*/ inputActionText
            input.onTextEdited()
        }
    }

    function setInputActionHint(inputActionText) {
        let query = albert.currentQuery()
        if (query.string().length !== 0){

            // Todo this feature breaaks a lot of handlers, cool thoguh, do it
            // let replacement = albert.currentQuery().trigger() + inputActionText
            let replacement = inputActionText

            if (inputActionText.length === 0)
                hint.text = ""
            else if (replacement.toLowerCase().startsWith(input.text.toLowerCase()))
                hint.text = replacement.slice(input.length, inputActionText.length)
            else
                hint.text = "  " + replacement
        }
    }
}
