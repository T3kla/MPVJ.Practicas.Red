/**
@file Manager.h

Contiene la declaraci�n de la clase CManager, Singleton que se encarga de
la gesti�n de la red del juego.

@see Net::CManager

@author David Llans�
@date Diciembre, 2010
*/
#ifndef __Net_Manager_H
#define __Net_Manager_H

#include <vector>
#include <map>

// Predeclaracion de clases
namespace Net {
	class CBuffer;
	class CServer;
	class CClient;
	class CConnection;
	class CFactory;
	class CFactoryEnet;
	class CPacket;
};

/**
Namespace que engloba lo relacionado con la parte de red.
(para m�s informaci�n ver @ref NetGroup).

@author David Llans�
@date Diciembre, 2010
*/
namespace Net
{
	/** 
	ID de identificaci�n en la red.
	*/
	typedef unsigned int NetID;

	/**
	Namespace para los tipos de IDs posibles.
	*/
	namespace ID
	{
		/** 
			Client identification that denotes the identification hasn't
			been initialized.
		*/
		enum { UNASSIGNED  = 0xFFFFFFFF,
			   SERVER      = 0x00000000,
			   FIRSTCLIENT = 0x00000001};

		/**
			Returns the next ID given the previous one.

			@param id The last assigned ID.
			@return New ID.
		*/
		NetID NextID(const NetID &id);

	}; // namespace ID

	enum NetMessageType {
		COMMAND,
		START_GAME,
		END_GAME,
		LOAD_MAP,
		LOAD_PLAYER,
		MAP_LOADED,
		PLAYER_LOADED,
		ENTITY_MSG,
		ASSIGNED_ID,
        BOMB_PLACED,
        BOMB_EXPLODED
	};



	/**
	Gestor de la red. Sirve como interfaz para que el resto de los
	proyectos interact�en con la red y no tengan que preocuparse de 
	la tecnolog�a espec�fica usada para su implementaci�n.
	<p>
	Esta clase debe ser espec�ficamente inicializada o configurada
	como cliente <em>o</em> como servidor. Sin embargo, desde fuera
	<em>puede usarse sin conocer su configuraci�n</em> y as� invocar 
	m�todos como CManager::tick() para que compruebe si se ha recibido 
	alg�n mensaje o invocar a CManager::send() cuando se quiera enviar 
	un mensaje al otro extremo de la conexi�n.

	@ingroup NetGroup

	@author David Llans�
	@date Diciembre, 2010
	*/
	class CManager
	{
	public:

		/**
		Interfaz que deben implementar las clases que quieran ser 
		notificadas de eventos de red.
		*/
		class IObserver{
		public:
			virtual void dataPacketReceived(Net::CPacket* packet)=0;
			virtual void connectionPacketReceived(Net::CPacket* packet)=0;
			virtual void disconnectionPacketReceived(Net::CPacket* packet)=0;
		};

    typedef std::pair<NetID, CConnection*> TConnectionPair;
    typedef std::map<NetID, CConnection*> TConnectionTable;

		/**
		Devuelve la �nica instancia de la clase CManager.
		
		@return �nica instancia de la clase CManager.
		*/
		static CManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CManager. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Ser� aqu� donde consultaremos si han llegado mensajes y de ser as� 
		los procesaremos o delegaremos su proceso al m�dulo correspondiente.
		*/
		void tick();

		/**
		Funci�n que sirve para enviar datos al otro lado de la conexi�n.
		Si se est� en modo cliente los datos se enviar�n al servidor
		mientras que si se encuentra en modo servidor la informaci�n se
		enviar� a todos los clientes registrados.

		@param data Datos a enviar.
    */
    void send(CBuffer* data, bool reliable);

    void sendAll(CBuffer* data, bool reliable);

		void activateAsServer(int port, int clients = 16, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		void activateAsClient(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		void connectTo(char* address, int port, int channels = 1, unsigned int timeout = 5000);

		void deactivateNetwork();


		void addObserver(IObserver*);

		void removeObserver(IObserver*);

		/**
		Devuelve el ID de red.
		
		@return El ID de red.
		*/
    NetID getID() { return _id; }

    const TConnectionTable& getConnections() const { return _connections; }

	protected:
		/**
		Constructor.
		*/
		CManager ();

		/**
		Destructor.
		*/
		~CManager();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

		void getPackets();
		
		bool internalData(Net::CPacket* packet);
		
		void connect(CConnection* connection);

		void disconnect(CConnection* connection);

	private:
		/**
		�nica instancia de la clase.
		*/
		static CManager* _instance;

		/**
		Factor�a de objetos de red
		*/
		Net::CFactory* _factory;

		/**
			 Servidor de red
		*/
		Net::CServer* _server;

		/**
			Cliente de red
		*/
		Net::CClient* _client;

		/**
			Conexiones de red. Es decir, el servidor visto desde el cliente
			o los clientes vistos desde el servidor. En el cliente solo se 
			usar� una y en el servidor tantas como clientes haya.
		*/
		TConnectionTable _connections;

		CConnection* getConnection(NetID id) {return (*_connections.find(id)).second;}

		bool addConnection(NetID id, CConnection* connection);

		bool removeConnection(NetID id);

		std::vector<IObserver*> _observers;

		std::vector<Net::CPacket*> _packets;

		/**
		ID de red una vez conectado.
		*/
		NetID _id;

		/**
		Siguiente ID de red que se asignar� al pr�ximo cliente. Solo se usa en modo 
		servidor.
		*/
		NetID _nextId;

		}; // class CManager

} // namespace Net

#endif // __Net_Manager_H
