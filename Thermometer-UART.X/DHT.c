#include "DHT.h"

void DHT_Start(DHT_Model_t modelo){
    if(modelo == DHT11){
        TRISAbits.TRISA0 = 0;      //Puerto RB0 como salida
        ANSELAbits.ANSELA0 = 0;
        LATAbits.LATA0 = 0;       //Mandamos cero
        __delay_ms(18);         //Esperamos 18 milisegundo
        LATAbits.LATA0 = 1;       //Mandamos uno
        __delay_us(40);         //Esperamos 40 microsegundos
        TRISAbits.TRISA0 = 1;      //Puerto como entrada
    }
    else if(modelo == DHT22){
        TRISAbits.TRISA0 = 0;      //Puerto RB0 como salida
        ANSELAbits.ANSELA0 = 0;
        LATAbits.LATA0 = 0;       //Mandamos cero
        __delay_ms(1);         //Esperamos 1 milisegundo
        LATAbits.LATA0 = 1;       //Mandamos uno
        __delay_us(40);         //Esperamos 40 microsegundos
        TRISAbits.TRISA0 = 1;      //Puerto como entrada
    }
}

void DHT_Check(DHT_Model_t modelo){
    unsigned char temp_x;
    if(modelo == DHT11){
        temp_x = 0;
        while((PORTAbits.RA0 == 1) && temp_x < 100){
            temp_x++;
        }
        __delay_us(80);
        temp_x = 0;
        while((PORTAbits.RA0 == 0) && temp_x < 100){
            temp_x++;
        }
        __delay_us(80);
        temp_x = 0;
        while((PORTAbits.RA0 == 1) && temp_x < 100){
            temp_x++;
        }
    }
    else if(modelo == DHT22){
        temp_x = 0;
        while((PORTAbits.RA0 == 1) && temp_x < 100){
            temp_x++;
        }
        __delay_us(80);
        temp_x = 0;
        while((PORTAbits.RA0 == 0) && temp_x < 100){
            temp_x++;
        }
        __delay_us(80);
        temp_x = 0;
        while((PORTAbits.RA0 == 1) && temp_x < 100){
            temp_x++;
        } 
    }        
}

unsigned char DHT_Read(DHT_Model_t modelo){
    unsigned char x = 0, data = 0;
    unsigned char temp_x;
    if(modelo == DHT11){
        for(x=0;x<8;x++){
            temp_x = 0;
            while((PORTAbits.RA0 == 0) && temp_x < 100){
                temp_x++;
            }
            __delay_us(35);
            if(PORTAbits.RA0){
                data = (unsigned char)((data<<1) | 1);
            }
            else{
                data = (unsigned char)(data<<1);
            }
            temp_x = 0;
            while((PORTAbits.RA0 == 1) && temp_x < 100){
                temp_x++;
            }
        }
    }
    else if(modelo == DHT22){
        for(x=0;x<8;x++){
            temp_x = 0;
            while((PORTAbits.RA0 == 0) && temp_x < 100){
                temp_x++;
            }
            __delay_us(35);
            if(PORTAbits.RA0){
                data = (unsigned char)((data<<1) | 1);
            }
            else{
                data = (unsigned char)(data<<1);
            }
            temp_x = 0;
            while((PORTAbits.RA0 == 1) && temp_x < 100){
                temp_x++;
            }
        }        
    }
    return data;
}

unsigned int DHT_GetTemp(DHT_Model_t modelo){
    unsigned char RH_Ent, RH_Dec, Temp_Ent, Temp_Dec, Chksum;
    unsigned int valor_dht=0;
    DHT_Start(modelo);
    DHT_Check(modelo);
    RH_Ent = DHT_Read(modelo);
    RH_Dec = DHT_Read(modelo);
    Temp_Ent = DHT_Read(modelo);
    Temp_Dec = DHT_Read(modelo);
    Chksum = DHT_Read(modelo); 
    if(modelo == DHT11){
        return(valor_dht + Temp_Ent);
    }
    else if(modelo == DHT22){
        valor_dht = (unsigned int)((Temp_Ent << 8) | Temp_Dec);
        return(valor_dht);
    }
    return 0;
}

unsigned int DHT_GetHumid(DHT_Model_t modelo){
    unsigned char RH_Ent, RH_Dec, Temp_Ent, Temp_Dec, Chksum;
    unsigned int valor_dht=0;
    DHT_Start(modelo);
    DHT_Check(modelo);
    RH_Ent = DHT_Read(modelo);
    RH_Dec = DHT_Read(modelo);
    Temp_Ent = DHT_Read(modelo);
    Temp_Dec = DHT_Read(modelo);
    Chksum = DHT_Read(modelo); 
    if(modelo == DHT11){
        return(valor_dht + RH_Ent);
    }
    else if(modelo == DHT22){
        valor_dht = (unsigned int)((RH_Ent << 8) | RH_Dec);
        return(valor_dht);
    }
    return 0;
}

DHT_Values_t DHT_GetBoth(DHT_Model_t modelo){
    static DHT_Values_t lectura;
    unsigned char RH_Ent, RH_Dec, Temp_Ent, Temp_Dec, Chksum;
    DHT_Start(modelo);
    DHT_Check(modelo);
    RH_Ent = DHT_Read(modelo);
    RH_Dec = DHT_Read(modelo);
    Temp_Ent = DHT_Read(modelo);
    Temp_Dec = DHT_Read(modelo);
    Chksum = DHT_Read(modelo);
    if(modelo == DHT11){
        lectura.DHT_Temp = Temp_Ent;
        lectura.DHT_Humid = RH_Ent;
    }
    else if(modelo == DHT22){
        lectura.DHT_Temp = (unsigned int)((Temp_Ent << 8) | Temp_Dec);
        lectura.DHT_Humid = (unsigned int)((RH_Ent << 8) | RH_Dec);
    }
    return lectura;
}