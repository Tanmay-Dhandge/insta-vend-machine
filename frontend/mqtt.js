const client = mqtt.connect(
'wss://YOUR_CLUSTER.s1.eu.hivemq.cloud:8884/mqtt',
{
username:'YOUR_USERNAME',
password:'YOUR_PASSWORD'
}
)

client.on('connect',()=>{
console.log("Connected to HiveMQ")
})
