# Import standard python modules.
import sys
import time
import serial

# This example uses the MQTTClient instead of the REST client
from Adafruit_IO import MQTTClient
from Adafruit_IO import Client, Feed
# holds the count for the feed
run_count = 0
# Set to your Adafruit IO username and key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_USERNAME = 
ADAFRUIT_IO_KEY      =

# Set to the ID of the feed to subscribe to for updates.
FEED_ID_receive_motor1 = 'TX_Motor1'
FEED_ID_receive_motor2 = 'TX_Motor2'
FEED_ID_receive_motor3 = 'TX_Motor3'
FEED_ID_receive_motor4 = 'TX_Motor4'
FEED_ID_receive_modo = 'modos'


FEED_ID_Send_motor1 = 'RX_Motor1'
FEED_ID_Send_motor2 = 'RX_Motor2'
FEED_ID_Send_motor3 = 'RX_Motor3'
FEED_ID_Send_motor4 = 'RX_Motor4'

# Define "callback" functions which will be called when certain events
# happen (connected, disconnected, message arrived).
def connected(client):
    """Connected function will be called when the client is connected to
    Adafruit IO.This is a good place to subscribe to feed changes. The client
    parameter passed to this function is the Adafruit IO MQTT client so you
    can make calls against it easily.
    """
    # Subscribe to changes on a feed named Counter.
    print('Subscribing to Feed {0}')
    client.subscribe(FEED_ID_receive_motor1)
    client.subscribe(FEED_ID_receive_motor2)
    client.subscribe(FEED_ID_receive_motor3)
    client.subscribe(FEED_ID_receive_motor4)
    client.subscribe(FEED_ID_receive_modo)
    print('Waiting for feed data...')

def disconnected(client):
    """Disconnected function will be called when the client disconnects."""
    sys.exit(1)

def message(client, feed_id, payload):
    """Message function will be called when a subscribed feed has a new value.
    The feed_id parameter identifies the feed, and the payload parameter has
    the new value.
    """
    print(f'Feed {feed_id} received new value: {payload}')

    if feed_id == FEED_ID_receive_motor1:
        # Slider de 0 a 90 → escalar a 0–255
        angle = int(payload)
        value = int(((angle - 90) / 90) * 255)
        miarduino.write(bytes(f"A{value}\n", 'utf-8'))
        print(f"Publicando en {FEED_ID_Send_motor1} valor: {payload}")
        client.publish(FEED_ID_Send_motor1, payload)

    elif feed_id == FEED_ID_receive_motor2:
        # Slider de 90 a 180 → escalar a 0–255
        angle = int(payload)
        value = int((angle / 90) * 255)

        miarduino.write(bytes(f"B{value}\n", 'utf-8'))
        client.publish(FEED_ID_Send_motor2, payload)

    elif feed_id == FEED_ID_receive_motor3:
        # Slider de 0 a 180 → escalar a 0–255
        angle = int(payload)
        value = int((angle / 180) * 255)
        miarduino.write(bytes(f"C{value}\n", 'utf-8'))
        client.publish(FEED_ID_Send_motor3, payload)

    elif feed_id == FEED_ID_receive_motor4:
        # Slider de 0 o 1 → escalar a 0 o 255
        value = 255 if int(payload) == 1 else 0
        miarduino.write(bytes(f"D{value}\n", 'utf-8'))
        client.publish(FEED_ID_Send_motor4, payload)

    elif feed_id == FEED_ID_receive_modo:
        miarduino.write(bytes(f"E{payload}\n", 'utf-8'))


    # Publish or "send" message to corresponding feed
    #print('Sendind data back: {0}'.format(payload))
    #client.publish(FEED_ID_Send, payload)

# Create an MQTT client instance. MQTT es un protocolo de comunicacion muy utilizado.
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

miarduino = serial.Serial(port='COM7', baudrate= 9600, timeout=0.1) #El timeout es para esperar 1ms para recibir algo


# Setup the callback functions defined above.
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

# Connect to the Adafruit IO server.
client.connect()

# The first option is to run a thread in the background so you can continue
# doing things in your program.
client.loop_background()

while True:
    """
    # Uncomment the next 3 lines if you want to constantly send data
    # Adafruit IO is rate-limited for publishing
    # so we'll need a delay for calls to aio.send_data()
    run_count += 1
    print('sending count: ', run_count)
    client.publish(FEED_ID_Send, run_count)
    """
    print('Running "main loop" ')


    print('Running "main loop" ')

    if miarduino.in_waiting:
        try:
            data = miarduino.readline().decode().strip()  # Ej: "A123"
            print(f"Datos recibidos del Arduino: {data}")

            if len(data) >= 2:
                motor_id = data[0]
                value = data[1:]
                value = int(value)

            if motor_id == 'A':
                angle = int((value / 255) * 90 + 90)
                client.publish(FEED_ID_Send_motor1, angle)

            elif motor_id == 'B':
                angle = int((value / 255) * 90)

                client.publish(FEED_ID_Send_motor2, angle)

            elif motor_id == 'C':
                angle = int((value / 255) * 180)
                client.publish(FEED_ID_Send_motor3, angle)

            elif motor_id == 'D':
                angle = 1 if value >= 128 else 0
                client.publish(FEED_ID_Send_motor4, angle)

        except Exception as e:
            print(f"Error leyendo del puerto serial: {e}")



    time.sleep(3)
