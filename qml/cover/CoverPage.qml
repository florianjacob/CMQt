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

CoverBackground {
	Column {
		anchors.centerIn: parent

		Image {
			anchors.horizontalCenter: parent.horizontalCenter
			source: "/usr/share/icons/hicolor/86x86/apps/CMQtExample.png"
		}

		Label {
			anchors.horizontalCenter: parent.horizontalCenter
			text: ledmatrix.connectionStateString
		}

		Label {
			anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Sending Frames…")
			visible: ledmatrix.available
		}

	}


	CoverActionList {
		enabled: ledmatrix.connectionState === LEDMatrix.UnconnectedState

		CoverAction {
			iconSource: "image://theme/icon-cover-search"
			onTriggered: ledmatrix.discover()
		}
	}

	CoverActionList {
		enabled: ledmatrix.connectionState === LEDMatrix.ConnectedState

		CoverAction {
			iconSource: "image://theme/icon-cover-cancel"
			onTriggered: ledmatrix.disconnectFromMachine()
		}
	}
}
