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

#include "ledmatrix.h"

#include <QtBluetooth/QBluetoothHostInfo>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QBluetoothAddress>

const QString LEDMatrix::NAME_LEDPI_TECO = "ledpi-teco";
const QString LEDMatrix::ADDR_LEDPI_TECO = "00:1A:7D:DA:71:07";
const QString LEDMatrix::NAME_EMULATOR = "izumo";
const QString LEDMatrix::ADDR_EMULATOR = "00:1A:7D:DA:71:0A";

LEDMatrix::LEDMatrix(const QString appName, char width, char height, char colorMode, QObject* parent) :
	QObject(parent)
	, m_socket{nullptr}
	, m_discoveryAgent{nullptr}
	, m_appName{appName}
	, m_width{width}
	, m_height{height}
	, m_colorMode{colorMode}
	, m_available{false}
	, m_connectionState{LEDMatrix::UnconnectedState}
{
}

LEDMatrix::~LEDMatrix()
{
}


void LEDMatrix::discover() {
	if (connectionState() == LEDMatrix::UnconnectedState) {
		changeConnectionState(LEDMatrix::DiscoveringState);

		/*
		 * TODO: Check for availability of bluetooth adapters.
			list of adapters:
			QList<QBluetoothHostInfo> localAdapters = QBluetoothLocalDevice::allDevices();
			default:
			QBluetoothHostInfo localAdapter = localAdapters.first();
			=> crashes when the list is empty
		*/

		if (!m_discoveryAgent) {
			m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

			connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
					this, SLOT(onDeviceDiscovered(QBluetoothDeviceInfo)));

			connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(onDiscoveryFinished()));
			connect(m_discoveryAgent, SIGNAL(canceled()), this, SLOT(onDiscoveryCanceled()));
			connect(m_discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
					this, SLOT(onDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error)));
		}


		// Start a discovery
		m_discoveryAgent->start();
		qDebug() << "Discovering Bluetooth devices..";

	} else {
		qDebug() << "discovery requested while not in unconnected state!!";
	}
}


// In your local slot, read information about the found devices
void LEDMatrix::onDeviceDiscovered(const QBluetoothDeviceInfo &device) {
	qDebug() << "Found new device:" << device.name() << '[' << device.address().toString() << ']';

	if (device.name() == NAME_LEDPI_TECO || device.name() == NAME_EMULATOR) {
		if (m_discoveryAgent) {
			m_discoveryAgent->stop();
		}
		connectToMachine(device.address());
	}
}


void LEDMatrix::onDiscoveryFinished() {
	// discovery finished but wasn't stopped due to a suitable device
	qDebug() << "Discovery finished without a suitable device.";
	m_discoveryAgent->deleteLater();
	m_discoveryAgent = nullptr;
	changeConnectionState(LEDMatrix::UnconnectedState);
}


void LEDMatrix::onDiscoveryCanceled() {
	// only called after .stop() => a suitable device was found
	qDebug() << "Discovery stopped.";
	m_discoveryAgent->deleteLater();
	m_discoveryAgent = nullptr;
}



void LEDMatrix::onDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error error) {
	qDebug() << "Discovery onError:" << error << m_discoveryAgent->errorString();
	m_discoveryAgent->deleteLater();
	m_discoveryAgent = nullptr;
	changeConnectionState(LEDMatrix::UnconnectedState);
}


void LEDMatrix::onStateChanged(QBluetoothSocket::SocketState state) {
	qDebug() << "Socket State changed, is now:" << state;
}


void LEDMatrix::onSocketError(QBluetoothSocket::SocketError error) {
	qDebug() << "Socket onError:" << error << m_socket->errorString();
	qDebug() << Q_FUNC_INFO;
}


void LEDMatrix::connectToMachine(const QBluetoothAddress& remoteAddress) {
	changeConnectionState(LEDMatrix::ConnectingState);

	if (!m_socket) {

		m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

		connect(m_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
		connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
		connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
		connect(m_socket, SIGNAL(error(QBluetoothSocket::SocketError)),
				this, SLOT(onSocketError(QBluetoothSocket::SocketError)));
		connect(m_socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)),
				this, SLOT(onStateChanged(QBluetoothSocket::SocketState)));

	}

	qDebug() << "Connecting to Machine..";
	m_socket->connectToService(remoteAddress, 16);
}


void LEDMatrix::connectToLedpiTeco() {
	connectToMachine(QBluetoothAddress(ADDR_LEDPI_TECO));
}


void LEDMatrix::connectToEmulator() {
	connectToMachine(QBluetoothAddress(ADDR_EMULATOR));
}


void LEDMatrix::disconnectFromMachine() {
	qDebug() << "Closing socket..";
	m_socket->close();
}


void LEDMatrix::readSocket() {
	if (!m_socket)
		return;

	while (m_socket->bytesAvailable() >= 2) {
		QByteArray response = m_socket->read(2);

		char handshakeResponse = response[0];
		m_maxFPS = response[1];

		// Check response. TODO: Handle non-zero (error) responses.
		if (handshakeResponse != 0) {
			qDebug() <<  "Response from server: Handshake error. (Code:" << handshakeResponse << ")";
		}
		qDebug() << "LEDMatrix is now available!";
		changeAvailable(true);

	}
}


void LEDMatrix::write(const QByteArray& msgBuffer) {
	m_socket->write(msgBuffer);
}


void LEDMatrix::onConnected() {
	qDebug() << "Connected! Peer Name:" << m_socket->peerName();
	changeConnectionState(LEDMatrix::ConnectedState);

	QByteArray handshake;
	handshake.resize(5 + m_appName.length());
	handshake[0] = VERSION;
	handshake[1] = m_width;
	handshake[2] = m_height;
	handshake[3] = m_colorMode;
	handshake[4] = (char) m_appName.length();
	handshake.append(m_appName);

	qDebug() << "Sending handshake..";
	write(handshake);
}


void LEDMatrix::onDisconnected() {
	qDebug() << "Disconnected";
	m_socket->deleteLater();
	m_socket = nullptr;
	changeConnectionState(LEDMatrix::UnconnectedState);
}


void LEDMatrix::changeConnectionState(LEDMatrix::ConnectionState state) {
	if (m_connectionState != state) {
		m_connectionState = state;
		emit connectionStateChanged();

		if (m_connectionState != LEDMatrix::ConnectedState) {
			changeAvailable(false);
		}
	}
}


void LEDMatrix::changeAvailable(bool available) {
	if (m_available != available) {
		m_available = available;
		emit availableChanged(available);
	}
}


char LEDMatrix::maxFPS() const {
	return m_maxFPS;
}


bool LEDMatrix::available() const {
	return m_available;
}


LEDMatrix::ConnectionState LEDMatrix::connectionState() const {
	return m_connectionState;
}


const QString LEDMatrix::connectionStateString() const {
	switch (m_connectionState) {
		case LEDMatrix::UnconnectedState:
			return tr("Not Connected");

		case LEDMatrix::DiscoveringState:
            return tr("Discovering…");

		case LEDMatrix::ConnectingState:
            return tr("Connecting…");

		case LEDMatrix::ConnectedState:
			return tr("Connected");

		default:
			qDebug() << "Unknown ConnectionState in string conversion";
			return tr("Unknown State");
	}
}


char LEDMatrix::width() const {
	return m_width;
}


char LEDMatrix::height() const {
	return m_height;
}
