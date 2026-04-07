const mqtt = require('mqtt')

// ❌ was: http://...  ✅ must be mqtts://
const client = mqtt.connect('mqtts://366c887e88bf40c496821b5e646d3a12.s1.eu.hivemq.cloud', {
  port: 8883,
  username: "hivemq.webclient.1775540663302",
  password: "7@1wDCrTa0jP<Wn>8yN,",
  // HiveMQ Cloud uses a valid CA — rejectUnauthorized: true is safe
  rejectUnauthorized: true,
})

client.on('connect', () => {
  console.log("Connected to broker")
  // ❌ was: vending/order  ✅ match frontend topic
  client.subscribe("vendbot/order")
  client.subscribe("vendbot/payment")
})

client.on('message', (topic, message) => {
  try {
    let data = JSON.parse(message)
    console.log(`[${topic}]`, data)
  } catch (e) {
    console.error("Bad JSON:", message.toString())
  }
})

client.on('error', (err) => console.error("MQTT error:", err))
client.on('offline', () => console.warn("Broker offline, retrying..."))
