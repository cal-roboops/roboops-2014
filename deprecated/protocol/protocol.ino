/*
*
*	TYPE	          #         SET	       	GROUP
*===============================================================
*	ANALOG INPUT:     6    (pin 0..5)	define zone => A
*	DIGITAL INPUT:    5    (pin 2..6)	define zone => B
*	DIGITAL OUTPUT:   4    (pin 7,8,12,13)	define zone => C	
*	PWM:              3    (pin 9..11)	define zone => D
*===============================================================
*
*/

//special
const int SYNC=100;

//all inputs
const int lenA=11;
int current_in=0;
int A[lenA] = {20,21,22,23,24,25,2,3,4,5,6};

//output
const int lenC=4;
int C[lenC] = {7,8,12,13};

const int lenD=3;
int D[lenD] = {9,10,11};

//wait
const int DELAY=5;


//setup ################################
void setup(){
	//set DIN ---------------------------
	for(int i=0;i<lenA;i++){
		if(A[1]<20){
			pinMode(A[i],INPUT);
		}  
	}
	//set DOUT --------------------------
	for(int i=0;i<lenC;i++){
		pinMode(C[i],OUTPUT);  
	}
	//set PWM --------------------------
	for(int i=0;i<lenD;i++){
		pinMode(D[i],OUTPUT);  
	}
	//begin serial  --------------------
	Serial.begin(19200);
}

//main loop #############################
void loop(){
	//set output
	parseCommand();
	//send back inputs
	readInputs();
}

//send to serial values #################
void printS(int pin,int value){
	Serial.write(pin);
	Serial.write(value);
}

//callback changed pins #################
void readInputs(){
	//increment counter
	current_in+=1;
	if(current_in>lenA){
		current_in=-1;
		//send resync BYTES
		printS(SYNC,SYNC);
	}else{
		//get current pin
		int pin= A[current_in];
		int value=0;
		//digital pins 
		if(pin<20){
			value=digitalRead(pin);
		//analog pins
		}else{
			value=analogRead(pin-20);
			//normalize (default 0-1023) 0-254
			value=value/4;
		}
		//callback
		printS(pin,value);
    }
    //wait
	delay(DELAY);  
}

//get a Byte ###########################
int getByte(){
	int res=0;
	while(1){
		if(Serial.available() > 0){
			res=Serial.read();
			return res;
		}
	}
}

//read from serial command ##############
void parseCommand(){
	if(Serial.available() > 0){
		//read first byte
		int pin = getByte();
		int value = getByte();
		//DOUT -------------------------------
		for(int i=0;i<lenC;i++){
			if(pin==C[i]){  
			    digitalWrite(pin,value);
			    return;
			}
		}
		//PWM --------------------------------
		for(int i=0;i<lenD;i++){
			if(pin==D[i]){  
			    analogWrite(pin,value);
			    return;
			}
		}
		//SYNC IN -----------------------------
		if(pin==SYNC){
			//check if value match
			if(value!=SYNC){
				//resync
				int value = getByte();
			}
		}
	}
}


