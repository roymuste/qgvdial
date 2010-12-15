import Qt 4.7

ListView {
    id: container

    signal sigCall (string number)
    signal sigText (string number)

    delegate: Flow {
        width: parent.width
        height: Math.max(textNumber.height, btnCall.height)

        Text {
            id: textNumber
            width: parent.width - btnCall.width - btnText.width
            text: type + "\t: " + number
            color: "white"
            font.pointSize: (Code.btnFontPoint () / 12)
        }

        TextButton {
            id: btnCall
            text: "Call"
            fontPoint: (Code.btnFontPoint () / 12)

            onClicked: container.sigCall(number)
        }
        TextButton {
            id: btnText
            text: "Text"
            fontPoint: (Code.btnFontPoint () / 12)

            onClicked: container.sigText(number)
        }
    }
}
