/*
 * Copyright © 2015 Florian Jacob
 *
 * This file is part of CMQt.
 *
 * CMQt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CMQt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CMQt. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import CMQt 0.1


Page {
	id: page

	SilicaFlickable {
		anchors.fill: parent

		PullDownMenu {
			MenuItem {
				text: qsTr("Discover ConnectionMachines")
				onClicked: ledmatrix.discover()
				visible: ledmatrix.connectionState === LEDMatrix.UnconnectedState
			}
			MenuItem {
				text: qsTr("Connect to Emulator")
				onClicked: ledmatrix.connectToEmulator()
				visible: ledmatrix.connectionState === LEDMatrix.UnconnectedState
			}
			MenuItem {
				text: qsTr("Connect to teco-CM")
				onClicked: ledmatrix.connectToLedpiTeco()
				visible: ledmatrix.connectionState === LEDMatrix.UnconnectedState
            }
			MenuItem {
				text: qsTr("Disconnect")
				onClicked: ledmatrix.disconnectFromMachine()
				visible: ledmatrix.connectionState === LEDMatrix.ConnectedState
			}
		}


		contentHeight: column.height

		Column {
			id: column

			width: page.width
			spacing: Theme.paddingLarge
			PageHeader {
				title: qsTr("ConnectionMachine")
			}

			Label {
				id: status
				anchors.horizontalCenter: parent.horizontalCenter
				text: ledmatrix.connectionStateString
			}

			Label {
				id: appstatus
				anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Sending Frames to CM…")
				visible: ledmatrix.available
			}
		}
	}
}


