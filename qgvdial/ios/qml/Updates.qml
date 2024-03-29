/*
qgvdial is a cross platform Google Voice Dialer
Copyright (C) 2009-2014  Yuvraaj Kelkar

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

import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    id: container

    height: mainColumn.height + 2

    Column {
        id: mainColumn

        anchors {
            top: parent.top
            left: parent.left
        }
        spacing: 5
        width: parent.width

        Item {
            width: parent.width
            height: (optContactsUpdate.height > edContactsUpdateFreq.height ? optContactsUpdate.height : edContactsUpdateFreq.height) + 4

            CheckBox {
                id: optContactsUpdate
                objectName: "OptContactsUpdate"
                text: "Update contacts"
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                }
            }//CheckBox
            TextField {
                id: edContactsUpdateFreq
                objectName: "EdContactsUpdateFreq"
                visible: optContactsUpdate.checked
                width: 200
                validator: IntValidator { bottom: 1; top: 1440 }
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                }
                placeholderText: "Minutes"
            }//TextField
        }//Item

        Item {
            width: parent.width
            height: (optInboxUpdate.height > edInboxUpdateFreq.height ? optInboxUpdate.height : edInboxUpdateFreq.height) + 4

            CheckBox {
                id: optInboxUpdate
                objectName: "OptInboxUpdate"
                text: "Update inbox"
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                }
            }//CheckBox
            TextField {
                id: edInboxUpdateFreq
                objectName: "EdInboxUpdateFreq"
                visible: optInboxUpdate.checked
                width: 200
                validator: IntValidator { bottom: 1; top: 1440 }
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                }
                placeholderText: "Minutes"
            }//TextField
        }//Item
    }// Column
}// Item (top level)
