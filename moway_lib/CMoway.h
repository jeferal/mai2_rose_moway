// Macros to make exporting easier (do not touch)
#include "RFModule.h"
#include <list>
#include <iostream>
#include <string>
#include <sstream>

#ifdef MLIB_EXPORTS
#define MLIB_API __declspec(dllexport)
#else

#define MLIB_API __declspec(dllimport)
#endif


	
	// Exportamos la clase
	class MLIB_API CMoway {

	public:
		CMoway(void);

		//Enums
		enum direction {BACKWARD,FORWARD};
		enum leds {LED_FRONT, LED_BRAKE, LED_TOP_GREEN, LED_TOP_RED};
		enum led_action {ON, OFF};
		enum axis {X, Y, Z};
		enum speaker_mode {SPEAKER_OFF, SPEAKER_ON, SPEAKER_TIME};
		
		/************************************************************/
		//CONNECTION METHODS
		/************************************************************/

		// Para comprobar si se ha conectado el moway
		bool CheckConnected();

		// Funcion para conectar con el robot
		// int robotid: Identificador del robot
		bool ConnectMoway(int robotId);

		// Funcion para establecer el retardo en milisegundos antes de enviar una órden
		// Por defecto son 100, para evitar solapamiento
		// Se puede jugar con este valor para intentar obtener mayor precisión cuando existen interferencias
		void SetSendDelay(int delay);

		// Funcion para desconectar el robot
		// bool disableRF. Indica si queremos desconectar el módulo rf del moway. Si se desconecta el rf del moway habrá que apagarlo y encenderlo otra vez para que pueda volver a recibir órdenes.
		bool DisconnectMoway(bool disableRF=false);


		/************************************************************/
		//ERROR HANDLING METHODS
		/************************************************************/

		// Devuelve un string con detalles sobre el error
		std::string GetErrors();

		// Vacia el string de errores
		void ResetErrors();

		/************************************************************/

		//EXTERNAL METHODS: COMMAND IMPLEMENTATION

		/************************************************************/
	
		// Funcion para definir el funcionamiento del altavoz
		// inf freq: Frecuencia del sonido. Valores posibles 0-255.
		/*			 Valor  Frecuencia
						 0  0,0000000 
						10  5681,8181818 
						20  2976,1904762 
						30  2016,1290323 
						40  1524,3902439 
						50  1225,4901961 
						60  1024,5901639 
						70  880,2816901 
						80  771,6049383 
						90  686,8131868 
					   100  618,8118812 
					   110  563,0630631
					   120  516,5289256
					   130  477,0992366
					   140  443,2624113
					   150  413,9072848
					   160  388,1987578
					   170  365,4970760
					   180  345,3038674
					   190  327,2251309
					   200  310,9452736
					   210  296,2085308
					   220  282,8054299
					   230  270,5627706
					   240  259,3360996
					   250  249,0039841
					   255  244,1406250
	    
		enum speaker_mode. Valores posibles: SPEAKER_OFF, SPEAKER_ON, SPEAKER_TIME. (SPEAKER_TIME indica que se emitirá un sonido durante el tiempo especificado en la variable time)
		int time. Indica el tiempo que estará sonando (solo funciona si mode = SPEAKER_TIME ). Valores posibles 0-255.(1 = 100ms)
		*/
		bool SetSpeaker (int freq, speaker_mode mode, int time=0);

		// Funcion para definir la velocidad del robot y la duracion del movimiento
		// int speed_motor_left: velocidad del motor izquierdo en %. Valores posibles de 0-100
		// int speed_motor_right: velocidad del motor derecho en %. Valores posibles de 0-100
		// direction dir_left: sentido del motor izquierdo. Valores posibles enum {BACKWARD, FORWARD}
		// direction dir_right: sentido del motor derecho. Valores posibles enum {BACKWARD, FORWARD}
		// int duration_right: Duracion del movimiento del motor derecho(1 = 100ms). Por defecto 0, movimiento infinito.
		// int duration_left: Duracion del movimiento del motor izquierdo(1 = 100ms). Por defecto 0, movimiento infinito.
		bool SetSpeed(int speed_motor_left, int speed_motor_right, direction dir_left=FORWARD, direction dir_right=FORWARD, int duration_left=0, int duration_right=0);

		// Funcion para definir la velocidad del robot y la distancia del movimiento
		// int speed_motor_left: velocidad del motor izquierdo en %. Valores posibles de 0-100
		// int speed_motor_right: velocidad del motor derecho en %. Valores posibles de 0-100
		// direction dir_left: sentido del motor izquierdo. Valores posibles enum {BACKWARD, FORWARD}
		// direction dir_right: sentido del motor derecho. Valores posibles enum {BACKWARD, FORWARD}
		// int distance_right: Distancia a recorrer por la rueda izquierda en mm. Valores posibles (0-255)
		// int distance_left: Distancia a recorrer por la rueda derecha en mm. Valores posibles (0-255)
		bool SetSpeedDistance(int speed_motor_left, int speed_motor_right, direction dir_left, direction dir_right, int distance_left, int distance_right);

		// int speed  velocidad de los motores de ambas ruedas en %. Valores posibles de 0-100
		// direction dir: sentido del movimiento de ambas ruedas. Valores posibles enum {BACKWARD, FORWARD}
		// int duration: Duracion del movimiento de ambos motores (1 = 100ms). Por defecto 0, movimiento infinito.
		bool GoStraight (int speed, direction dir=FORWARD, int duration=0);

		// int speed  velocidad de los motores de ambas ruedas en %. Valores posibles de 0-100
		// direction dir: sentido del movimiento de ambas ruedas. Valores posibles enum {BACKWARD, FORWARD}
		// int distance. Distancia a recorrer en mm. Valores posibles (0-255)
		bool GoStraightDistance (int speed, direction dir, int distance);

		// Funcion para parar el movimiento del robot
		bool MotorStop();

		// Funcion para cambiar el estado de los leds
		// enum led led_number: El led. Valores posibles LED_FRONT, LED_BRAKE, LED_TOP_RED, LED_TOP_GREEN
		// enum led_action: Para apagar o encender el led. Valores posibles ON,OFF
		bool ChangeLEDState(CMoway::leds led_number, CMoway::led_action action_number);

		// Obtiene la velocidad de las ruedas en %. Valores posibles de 0-100
		bool ReadSpeed(int *speed_motor_left, int *speed_motor_right);

		// Obtiene la lectura analógica de los sensores de proximidad
		// Obtiene valores entre 0 y 255 (0 no hay ningún obstáculo, 255 el sensor está pegado al obstáculo)
		// int left:			Valor del sensor izquierdo. Valores posibles (0-255)
		// int center_left:		Valor del sensor centro-izquierdo. Valores posibles (0-255)
		// int center_right:	Valor del sensor centro-derecho. Valores posibles (0-255)
		// int right:			Valor del sensor derecho. Valores posibles (0-255)
		bool ReadProximitySensors(int* left, int *center_left, int* center_right, int* right);

		// Obtiene la lectura digital de los sensores de proximidad
		// Obtiene valores entre 0 y 1. (0 no hay obstáculo 1 el sensor ha detectado obstáculo)
		// int left:			Valor del sensor izquierdo. Valores posibles (0-1)
		// int center_left:		Valor del sensor centro-izquierdo. Valores posibles (0-1)
		// int center_right:	Valor del sensor centro-derecho. Valores posibles (0-1)
		// int right:			Valor del sensor derecho. Valores posibles (0-1)
		bool CheckProximitySensors(int* left, int *center_left, int* center_right, int* right);
		
		// Obtiene la lectura analógica de los sensores de línea
		// Obtiene valores entre 0 y 255 (0 no hay ninguna línea, 255 el sensor está pegado a la línea)
		// int left:			Valor del sensor izquierdo. Valores posibles (0-255)
		// int right:			Valor del sensor derecho. Valores posibles (0-255)
		bool ReadLineSensors(int* left, int* right);

		// Obtiene la lectura digital de los sensores de línea
		// Obtiene valores entre 0 y 1. (0 no hay ninguna línea 1 el sensor ha detectado una línea)
		// int left:			Valor del sensor izquierdo. Valores posibles (0-1)
		// int right:			Valor del sensor derecho. Valores posibles (0-1)
		bool CheckLineSensors(int* left, int* right);

		// Devuelve la cantidad de luz detectada en %. Valores posibles 0-100 (0 oscuridad)
		bool ReadAmbientLightSensor(int* lightsensor);

		// Devuelve el estado de la batería en %. Valores posibles 0-100
		bool ReadBatteryStatus(int* battery);

		// Función para leer el valor analógico micrófono
		// int* noise: El ruido detectado por el moway. Valores posibles 0-255
		bool ReadMicSensor(int* noise);

		// Función para leer el valor digital micrófono 
		// int* noise: El ruido detectado por el moway. Valores posibles 0-1
		bool CheckMicSensor(int* noise);

		// Devuelve la temperatura en ºC. Error estimado +-5ºC
		bool ReadTemp(int* temp);

		// Pone a 0 el contador de distancia
		bool MotorDistanceReset();

		// Pone a 0 el cuentakilómetros
		bool MotorKmReset();
		
		// pone a cero el tiempo
		bool MotorTimeReset();

		// Obtiene la distancia recorrida por las ruedas desde el último comando en mm. Valores posibles 0-255
		bool ReadMotorDistances(int* left, int* right);

		// Obtiene el tiempo transcurrido desde el último comando 100ms/bit. Valores posibles 0-255
		bool ReadMotorTime(int* time);

		// Obtiene el cuenta kilometros 1mm/bit. Valores posibles 0-512
		bool CMoway::ReadMotorKM(int* KM);

		// Obtiene la rotacion de las ruedas desde el último comando. Valores posibles de 0-360
		bool ReadWheelRotation(int* left, int* right);

		// Aceleracion sufrida por el robot en los ejes x, y ,z. Valores de 0 a 255 (0 -2G, 255 2G) +-0.0156G/bit
		bool ReadAccelerator(int* result, CMoway::axis eje);

		// Destructor
		~CMoway();

		// Métodos y variables privados ( no accesibles )
		private:
			bool  SendCommand(uchar* commands, int commandsSize, int* result, char* message, int resultSize=1);
			bool  ReadProximity(int* left, int *center_left, int* center_right, int* right, bool digital=false);
			bool  ReadLine(int* left, int* right, bool digital = false);
			bool  ReadMic(int* noise, bool digital=false);
			void  handleError(std::string message);
			void  LedsLoop();

			std::string error;
			private: RFModule rf;
			private: bool connected;		
			private: int delay;
	};	
