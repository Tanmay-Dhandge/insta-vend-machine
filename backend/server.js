const mqtt = require('mqtt')

const client = mqtt.connect(
'mqtts://YOUR_CLUSTER.s1.eu.hivemq.cloud:8883',
{
username:"USERNAME",
password:"PASSWORD"
})

client.on('connect',()=>{

console.log("Connected to broker")

client.subscribe("vending/order")

})

client.on('message',(topic,message)=>{

let data = JSON.parse(message)

console.log("New Order:",data)

})
