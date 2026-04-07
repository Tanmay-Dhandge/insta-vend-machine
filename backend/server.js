const mqtt = require('mqtt')

const client = mqtt.connect(
  'mqtts://366c887e88bf40c496821b5e646d3a12.s1.eu.hivemq.cloud:8883',
  {
    username: "hivemq.webclient.1775540663302",
    password: "7@1wDCrTa0jP<Wn>8yN,",
    rejectUnauthorized: true
  }
)

client.on('connect', () => {
  console.log("✅ Connected to HiveMQ")
  client.subscribe("vendbot/order")
})

client.on('message', (topic, message) => {
  try {
    const data = JSON.parse(message.toString())
    console.log("🧾 Order:", JSON.stringify(data, null, 2))
  } catch (e) {
    console.log("❌ Invalid JSON:", message.toString())
  }
})

client.on('error', err => {
  console.log("MQTT Error:", err)
})
