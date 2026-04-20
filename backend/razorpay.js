// razorpay.js

const Razorpay = require('razorpay');

// Initialize Razorpay
const razorpay = new Razorpay({
    key_id: 'YOUR_KEY_ID', // Replace with your actual key_id
    key_secret: 'YOUR_KEY_SECRET' // Replace with your actual key_secret
});

// Create payment order
function createOrder(amount) {
    return new Promise((resolve, reject) => {
        const options = {
            amount: amount * 100, // Amount in smallest currency unit
            currency: 'INR',
            receipt: 'receipt#1',
            payment_capture: 1 // Automatically capture payment
        };
        razorpay.orders.create(options, (err, order) => {
            if (err) {
                reject(err);
            }
            resolve(order);
        });
    });
}

module.exports = { createOrder };