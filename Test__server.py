from pyModbusTCP.server import ModbusServer, DataBank
from time import sleep
from random import uniform

#create an instant of ModbusServer
server = ModbusServer("127.0.0.1", 12345, no_block = True)

try:
    print("Server start")
    server.start()
    print("Server online")

    state = [0]
    while True:
        DataBank.set_word(0, [int(uniform(0,100))])
        if state != DataBank.get_word(1):
            state = DataBank.get_work(1)
            print("new value"+str(state))
            sleep(0.5)
except:
    print("Shutdown server")
    server.stop()
    print("Server offine")
