#ifndef IoExtender_h
#define IoExtender_h

#include "Arduino.h"

#define IO_EXTENDER_PORT_NUMBER 16
#define IO_EXTENDER_START_ADDRESS 0x20
#define IO_EXTENDER_NUMBER 7

class IoExtender {
  public: 
    IoExtender();    

    uint8_t init(uint8_t address);

    /**
     * Metoda przydatna w przypadku kiedy domyślnym stanem na przycisku jest 1, 
     * a przy wciśnięciu mamy 0.
     */
    void invertPortsPolarity();
    uint16_t getPortsStatus();
    void getChanges(int8_t changes[]);
    
  private:
    static boolean _wire_init;
    uint8_t _address;
    uint16_t _portsStatus;
    boolean _init;

    uint16_t getCurrentPortStatus();
    
};


#endif
