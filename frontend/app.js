function buyProduct(product,price)
{

let order = {

order_id : "ORD"+Math.floor(Math.random()*10000),

product : product,

price : price,

payment : "success",

time : new Date().toISOString()

}

let message = JSON.stringify(order)

client.publish("vending/order",message)

alert("Order Sent!")

}
