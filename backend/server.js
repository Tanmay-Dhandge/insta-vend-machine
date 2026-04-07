const mqtt = require('mqtt')

const client = mqtt.connect(
'http://366c887e88bf40c496821b5e646d3a12.s1.eu.hivemq.cloud:8883',
{
username:"hivemq.webclient.1775540663302",
password:"7@1wDCrTa0jP<Wn>8yN,"
})

client.on('connect',()=>{

console.log("Connected to broker")

client.subscribe("vending/order")

})

client.on('message',(topic,message)=>{

let data = JSON.parse(message)

console.log("New Order:",data)

})
