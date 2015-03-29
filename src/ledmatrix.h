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

#ifndef LEDMATRIX_H
#define LEDMATRIX_H


#include <QtCore/QObject>
#include <QtBluetooth/QBluetoothSocket>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>



class LEDMatrix : public QObject {
	Q_OBJECT
	Q_ENUMS(ConnectionState)
	Q_PROPERTY(ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)
	Q_PROPERTY(QString connectionStateString READ connectionStateString NOTIFY connectionStateChanged)
	Q_PROPERTY(bool available READ available NOTIFY availableChanged)
	Q_PROPERTY(bool width READ width)
	Q_PROPERTY(bool height READ height)


public:

	enum ConnectionState {
		UnconnectedState,
		DiscoveringState,
		ConnectingState,
		ConnectedState,
	};

	explicit LEDMatrix(const QString appName, char width, char height, char colorMode, QObject* parent = nullptr);
	~LEDMatrix();

	char maxFPS() const;
	char width() const;
	char height() const;
	bool available() const;
	LEDMatrix::ConnectionState connectionState() const;
	const QString connectionStateString() const;


public slots:
	/** Tries to find bluetooth devices named "ledpi-teco" or your emulator's hostname and tries to connect with them. */
	void discover();
	/** Connect to the given bluetooth address without discovering. */
	void connectToMachine(const QBluetoothAddress& remoteAddress);
	/** Connects to the hardcoded QBluetoothAddress of the ledpi-teco ConnectionMachine */
	void connectToLedpiTeco();
	/** Connects to the hardcoded QBluetoothAddress of an emulator, i.e. your home machine */
	void connectToEmulator();

	void write(const QByteArray& msgBuffer);

	void disconnectFromMachine();


signals:
	void connectionStateChanged();

	void availableChanged(bool available);




private slots:
	void onDeviceDiscovered(const QBluetoothDeviceInfo& device);
	void onDiscoveryFinished();
	void onDiscoveryCanceled();
	void onDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error error);

	void onConnected();
	void readSocket();
	void onDisconnected();

	void onStateChanged(QBluetoothSocket::SocketState state);
	void onSocketError(QBluetoothSocket::SocketError error);


	void changeConnectionState(const LEDMatrix::ConnectionState state);
	void changeAvailable(const bool available);

private:
	static const char VERSION = 1;
	static const QString NAME_LEDPI_TECO;
	static const QString ADDR_LEDPI_TECO;
	static const QString NAME_EMULATOR;
	static const QString ADDR_EMULATOR;

	QBluetoothSocket* m_socket;
	QBluetoothDeviceDiscoveryAgent* m_discoveryAgent;

	const QString m_appName;
	char m_maxFPS;
	char m_width;
	char m_height;
	char m_colorMode;

	bool m_available;
	LEDMatrix::ConnectionState m_connectionState;


};

#endif // LEDMATRIX_H
