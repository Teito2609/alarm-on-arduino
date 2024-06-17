#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

RTC_DS1307 RTC;
LiquidCrystal_I2C lcd(0x27,16,2); // utilizo el 0x27 porq es mi direccion de memoria 

int menu = 0;
int sonador=0;
int i=0;

byte cuerpo[8] = { B01110,
B01110,
B00100,
B11111,
B00100,
B01010,
B10001,
B00000,
};          //personita

typedef struct{
  int hora=0;
  int minuto=0;
}Alarma;

Alarma alarma[5];     //lista de alarmas

uint8_t button_estate = HIGH;
int   VRx = 0;
int   VRy = 0;
const int SW=2;
int   btnReadmenu, btn_push, mapX, mapXnegado, mapY,mapYnegado, btnRead, btnRead2, numero_menu = 1, numero2_menu = 1, sonador_menu = 0, sonador_menu2 = 0;
int line[2]={0,1}, line_bk[2], line_bk2[2], index, index2;
int caract[20]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}, caract_bk[20], caract_bk2[20];

//variables del joystick

void setup(){
  
  lcd.init();// Inicializacion el LCD
  lcd.backlight(); // Enciende la luz del fondo.
  
  lcd.createChar(6,cuerpo);

  Wire.begin(); // Inicia el puerto I2C
  RTC.begin(); // Inicia la comunicacion con el RTC
  RTC.adjust(DateTime(__DATE__, __TIME__));  // Establece la fecha y hora (Comentar una vez establecida la hora)

  pinMode(SW, INPUT_PULLUP);
}

void loop() {
  if(menu == 0){
    menu_principal(menu);
    leeJoystickmenu();
    selec_menu_principal();

    DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC

    if (alarma[0].minuto == now.minute() && alarma[0].hora == now.hour()){
      lcd.setCursor(0,0);
      lcd.print("Alarma sonando!");
      digitalWrite(7,1);
      delay(20000);
      digitalWrite(7,0);
      delay(60000);
    }
    else  if (alarma[1].minuto == now.minute() && alarma[1].hora == now.hour()){
      lcd.setCursor(0,0);
      lcd.print("Alarma sonando!");
      digitalWrite(7,1);
      delay(20000);
      digitalWrite(7,0);
      delay(60000);
    }
    else  if (alarma[2].minuto == now.minute() && alarma[2].minuto == now.hour()){
      lcd.setCursor(0,0);
      lcd.print("Alarma sonando!");
      digitalWrite(7,1);
      delay(20000);
      digitalWrite(7,0);
      delay(60000);
    }
    else  if (alarma[3].minuto == now.minute() && alarma[3].minuto == now.hour()){
      lcd.setCursor(0,0);
      lcd.print("Alarma sonando!");
      digitalWrite(7,1);
      delay(20000);
      digitalWrite(7,0);
      delay(60000);
    }
    else if (alarma[4].minuto == now.minute() && alarma[4].minuto == now.hour()){
      lcd.setCursor(0,0);
      lcd.print("Alarma sonando!");
      digitalWrite(7,1);
      delay(20000);
      digitalWrite(7,0);
      delay(60000); 
    }
  }
  else if(menu == 1){
    menu_list_alarma(menu);
    leeJoystick();
    selec_menu();
    if(!sonador_menu){
      printMenu();
    }
    if(sonador_menu2){ 
      printSubMenu();
    }
  }
  else if(menu == 2){
    sonador = 5;
    if(numero2_menu == 1){
      sonador = 0;
    }
    else if(numero2_menu == 18){
      sonador = 1;
    }
    else if(numero2_menu == 15){
      sonador = 2;
    }
    else if(numero2_menu == 12){
      sonador = 3;
    }
    else if(numero2_menu == 9){
      sonador = 4;
    }
    menu_set_alarma(sonador);
    leeJoystick2();
    selec_menu2();
  }
}

void menu_principal(int* menu){

  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC

  lcd.setCursor(0,0);
  lcd.print("D: ");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print(" ");
  lcd.write (byte (6)); //dia actual

  lcd.setCursor(0,9);
  lcd.print("T: ");
  if(now.hour() < 10){
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  if(now.minute() < 10){
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC); //tiempo actual

  delay(1000); // La informacion se actualiza cada 1 segundo
  lcd.clear();
}

void menu_list_alarma(int* menu){
  printMenu();
}

void menu_set_alarma(int sonador){
  if(sonador < 5){
    lcd.setCursor(0,0);
    lcd.print("Alarma N ");
    lcd.print(sonador + 1);

    lcd.setCursor(0,1); 
    lcd.print(">");
    if(alarma[sonador].hora < 10){
      lcd.print("0");
    }
    lcd.print(alarma[sonador].hora);
    lcd.print(":");
    if(alarma[sonador].minuto < 10){
      lcd.print("0");
    }
    lcd.print(alarma[sonador].minuto);
    lcd.print("               ");
  }
  else if(sonador == 5){
    lcd.setCursor(0,0);
    lcd.print("no se eligio");
    lcd.setCursor(0,1);
    lcd.print("alarma             ");
  }
}

uint8_t flancoSubida(){
  uint8_t valor_nuevo = digitalRead(SW);
  uint8_t result = button_estate != valor_nuevo && valor_nuevo == 1;
  button_estate = valor_nuevo;
  return result;
}

int leeJoystickmenu(){
  VRx = analogRead(A0); 
  VRy = analogRead(A1);
  mapX = map(VRx, 1023, 0, 0, 7);
  mapXnegado = map(VRx, 0, 1023, 0, 7); // izquierda-derecha [1-2]
  mapY = map(VRy, 0, 1023, 0, 7);
  mapYnegado = map(VRy, 1023, 0, 0, 7); // arriba - abajo    [3-4] 
  btn_push = flancoSubida();      // push-Enter        [ 5 ]
                                  // Sin mover Joystick[ 0 ]
  if(mapX == 0){btnReadmenu = 1;}
  else if(mapXnegado == 0) {btnReadmenu = 2;}
       else if(mapYnegado == 0) {btnReadmenu = 3;}
            else if(mapY == 0) {btnReadmenu = 4;}
                 else if(btn_push == 1) {btnReadmenu = 5;}
                      else btnReadmenu = 0;                  
  return btnReadmenu;                      
}

void selec_menu_principal(){
  switch(btnReadmenu){
  case 4:// HACIA ARRIBA
  break;
  case 3:// HACIA ABAJO
        menu = 1;
        lcd.clear();
  break;           
  case 2:// HACIA LA DERECHA
  break;
  case 1://HACIA LA IZQUIERDA
  break;
  case 5:
  break;
  case 0:
       
  break;  
  }
}

int leeJoystick(){
  VRx = analogRead(A0); 
  VRy = analogRead(A1);
  mapX = map(VRx, 1023, 0, 0, 7);
  mapXnegado = map(VRx, 0, 1023, 0, 7); // izquierda-derecha [1-2]
  mapY = map(VRy, 0, 1023, 0, 7);
  mapYnegado = map(VRy, 1023, 0, 0, 7); // arriba - abajo    [3-4] 
  btn_push = flancoSubida();      // push-Enter        [ 5 ]
                                  // Sin mover Joystick[ 0 ]
  if(mapX == 0){btnRead = 1;}
  else if(mapXnegado == 0) {btnRead = 2;}
       else if(mapYnegado == 0) {btnRead = 3;}
            else if(mapY == 0) {btnRead = 4;}
                 else if(btn_push == 1) {btnRead = 5;}
                      else btnRead = 0;                  
  return btnRead;                      
}
//Desliza menu con Joystick en LCD
void selec_menu(){
  switch(btnRead){
  case 4:// HACIA ARRIBA
        menu = 0;
        lcd.clear();
  break;
  case 3:// HACIA ABAJO
  break;           
  case 2:// HACIA LA DERECHA
        menu_derecha();
        numero2_menu+=1;
        if(numero2_menu > 20){
          numero2_menu =1;
        }

        lcd.clear();
  break;
  case 1://HACIA LA IZQUIERDA
        menu_izquierda();
        numero2_menu-=1;
        if(numero2_menu < 1){
          numero2_menu =20;
        }
       
        lcd.clear();
  break;
  case 5:
        menu = 2;
        lcd.clear();
  break;
  case 0:
       
  break;  
  }
}
void menu_derecha(){
    for(int j=0; j < 20; j++){
      index2 = j+1;
      caract_bk2[j]=caract[j];
      if (index2 > 19){ 
        caract[j]=caract_bk2[j-19];
      }
      else{
        caract[j]=caract[j+1];
      }
  }
}
void menu_izquierda(){
    for(int i=19; i>-1; i--) {
     index = i-1;
     caract_bk[i] = caract[i];  
     if(index < 0){ 
      caract[i] = caract_bk[i+19];
    }
     else{ 
      caract[i] = caract[i-1];
    }
    sonador=i;
  }
}

void printMenu(){
 
  lcd.setCursor(0,1);
  lcd.print(">");
    lcd.setCursor(1,0);
    lcd.print("Alarma N:");
    lcd.setCursor(caract[1],1);
    lcd.print("1");
    lcd.setCursor(caract[4],1);
    lcd.print("2");
    lcd.setCursor(caract[7],1);
    lcd.print("3");
    lcd.setCursor(caract[10],1);
    lcd.print("4");
    lcd.setCursor(caract[13],1);
    lcd.print("5");
  }

void printSubMenu(){

}

int leeJoystick2(){
  VRx = analogRead(A0); 
  VRy = analogRead(A1);
  mapX = map(VRx, 0, 1023, 0, 7);
  mapXnegado = map(VRx, 1023, 0, 0, 7); // izquierda-derecha [1-2]
  mapY = map(VRy, 0, 1023, 0, 7);
  mapYnegado = map(VRy, 1023, 0, 0, 7); // arriba - abajo    [3-4] 
  btn_push = flancoSubida();      // push-Enter        [ 5 ]
                                  // Sin mover Joystick[ 0 ]
  if(mapX == 0){btnRead2 = 1;}
  else if(mapXnegado == 0) {btnRead2 = 2;}
       else if(mapYnegado == 0) {btnRead2 = 3;}
            else if(mapY == 0) {btnRead2 = 4;}
                 else if(btn_push == 1) {btnRead2 = 5;}
                      else btnRead2 = 0;                  
  return btnRead2;                      
}
void selec_menu2(){
  switch(btnRead2){
  case 4:// HACIA ARRIBA
        if(alarma[sonador].hora >= 0 && alarma[sonador].hora <= 23){
          delay(400);
          alarma[sonador].hora+=1;
        }

        lcd.clear();
  break;
  case 3:// HACIA ABAJO
        if(alarma[sonador].hora >= 1 && alarma[sonador].hora <= 24){
          delay(400);
          alarma[sonador].hora-=1;
        }

        lcd.clear();
  break;           
  case 2:// HACIA LA DERECHA
        if(alarma[sonador].minuto >= 0 && alarma[sonador].minuto <= 59){
          delay(100);
          alarma[sonador].minuto+=1;
        }

        lcd.clear();
  break;
  case 1://HACIA LA IZQUIERDA
        if(alarma[sonador].minuto >= 1 && alarma[sonador].minuto <= 60){
          delay(100);
          alarma[sonador].minuto-=1;
        }
       
        lcd.clear();
  break;
  case 5:
        menu = 0;
        lcd.clear();
  break;
  case 0:
       
  break;  
  }
}