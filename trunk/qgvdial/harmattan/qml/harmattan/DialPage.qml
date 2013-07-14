/*
qgvdial is a cross platform Google Voice Dialer
Copyright (C) 2009-2013  Yuvraaj Kelkar

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Contact: yuvraaj@gmail.com
*/

import QtQuick 1.1
import com.nokia.meego 1.1

Page {
    id: container
    tools: commonTools

    signal sigHaptic
    property real toolbarHeight: 50

    TextField {
        id: numberField

        inputMethodHints: Qt.ImhDialableCharactersOnly
        placeholderText: "Enter number here"

        anchors {
            top: parent.top
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }

        width: parent.width
        height: 290
        font.pointSize: 14
    }

    Keypad {
        id: keypad

        anchors {
            top: numberField.bottom
            topMargin: 2
            horizontalCenter: parent.horizontalCenter
        }

        width: parent.width - 8
        height: parent.height * 7/18

        onBtnClick: {
            var origStart = numberField.selectionStart;
            var result = numberField.text.substr(0, origStart);
            result += strText;
            result += numberField.text.substr(numberField.selectionEnd);
            numberField.text = result;
            numberField.cursorPosition = origStart + strText.length;
        }
        onSigHaptic: container.sigHaptic();
    }

    ButtonRow {
        id: btnRow
        anchors {
            top: keypad.bottom
            topMargin: 8
            horizontalCenter: parent.horizontalCenter
        }

        exclusive: false
        Button {
            text: "Text"
            height: 100
            font.pixelSize: 35
        }
        Button {
            text: "Call"
            height: 100
            font.pixelSize: 35
        }
    }
}
