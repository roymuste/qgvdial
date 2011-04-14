import Qt 4.7
import "helper.js" as Code

Rectangle {
    id: button
    border.color: activeFocus?"orange":"black"
    smooth: true

    radius: ((height + width) / 20);
    height: mText.height + 4

    // Main text in the button
    property string mainText: "2"
    property real mainPixelSize: 100

    // Aliases to the mText
    property alias aliasTextElide: mText.elide

    // Button emits clicks, but we also mention what is the text to display
    signal clicked(string strText)
    signal pressHold(string strText)

    gradient: Gradient {
        GradientStop { id: gradientStop; position: 0.0; color: "azure" }
        GradientStop { position: 1.0; color: palette.button }
    }
    SystemPalette { id: palette }

    onWidthChanged: updateTextWidth();
    function updateTextWidth() {
        if (mText.elide != Text.ElideNone) {
            // Without this, the elide value is pointless
            mText.width = button.width
        }
    }

    // The main text
    Text {
        id: mText
        text: button.mainText
        color: "black"

        // This is specifically for the current phone number button
        onTextChanged: button.updateTextWidth();

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        font.pixelSize: mainPixelSize
    }// Text

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: button.clicked(mainText);
        onPressAndHold: button.pressHold(mainText);
    }// MouseArea

    Keys.onReturnPressed: button.clicked(mainText);
    Keys.onSpacePressed: button.clicked(mainText);

    states: State {
        name: "pressed"
        when: mouseArea.pressed
        PropertyChanges { target: gradientStop; color: "orange" }
    }
}// Rectangle