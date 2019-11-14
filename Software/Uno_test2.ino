int lastdata[9] = {0,0,0,0,0,0,0,0,0};

void setup() {

  // put your setup code here, to run once:

  Serial.begin(9600);

}



int* getBuffer(int* lastdata){

    /***************************************************

     * start char '$'

     * acceleration end char '%'

     * stepper1 end char '@'

     * stepper2 end char '#'

     * stepper3 end char '!'

     * stepper4 end char '&'

     * $acceleration%dir1,fqc1@dir2,fqc2#dir3,fqc3!dir4,fqc4&

    ***************************************************/

    int i = 1;

    int dataindex = 0;

    int flag; 

    int* data = lastdata;

    while(Serial.available()){

    delay(3);

    if(Serial.available()){

      String buff = Serial.readString();

      if(buff[0] != '$')

        break;

      while(i < buff.length()){

//        if(buff[i] == ','){
//
//          flag = i;
//
//        }

        if(buff[i] == '@'){

          data[0] = buff[i - 2] >= 64? 1 : 0;

          data[1] = ((buff[i - 2] - 64) << 8) + buff[i - 1];

        }

        if(buff[i] == '#'){

          data[2] = buff[i - 2] >= 64? 1 : 0;

          data[3] = ((buff[i - 2] - 64) << 8) + buff[i - 1];

          

        }

        if(buff[i] == '!'){

          data[4] = buff[i - 2] >= 64? 1 : 0;

          data[5] = ((buff[i - 2] - 64) << 8) + buff[i - 1];

//          Serial.println(data[4]);

//          delayMicroseconds(300);

//          Serial.println(data[5]);

//          delayMicroseconds(300);

        }

        if(buff[i] == '&'){

          data[6] = buff[i - 2] >= 64? 1 : 0;

          data[7] = ((buff[i - 2] - 64)<< 8) + buff[i - 1];

        }

        if(buff[i] == '%'){

          data[8] = buff[i - 1];

        }

        i ++;

      }

      

    }

  }

  return data;

}

String sep = "=======================";

void loop() {

  // put your main code here, to run repeatedly:

  int* getbuff = getBuffer(lastdata);

  for(int i = 0; i < 9; i ++){

    Serial.println(getbuff[i]);

    delayMicroseconds(300);

  }

  Serial.println(sep);

}
