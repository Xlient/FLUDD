const accountSid =  process.env.TWILIO_ACCOUNT_SID;
const authToken =   process.env.TWILIO_AUTH_TOKEN;
const myphone = process.env.MY_PHONE_NUMBER;
const twilioNumber = process.env.TWILIO_NUMBER;
const client = require('twilio')(accountSid, authToken);
const express = require('express');

const app = express()
app.use(express.urlencoded({extended: true}));
app.use(express.json())
const port = 3000
let isUrgent = false;
let UserResponded = true;
let currentTime;


app.get('/', (req, res) => {
 //TODO
  res.send(200)
})

app.post('/', (req, res) => {
 let msg = ``;

  const waterLevelStatus = req.body.waterLevelStatus;
  const waterLeakStatus = req.body.waterLeakStatus;
  const waterDistance = Number(req.body.waterDistance);
  const time = req.body.timeReceived;
  const date = req.body.dateReceived;

  if(waterLevelStatus !== "All clear" || waterLeakStatus !== "All clear")
  {
    msg =` Alert 🚨
     Water Level : ${waterLevelStatus} 
     Water Leaks : ${waterLeakStatus} 
     Water is ${waterDistance} ft away from the floor of the basement
     at ${time} on ${date}. Please check your Sump pump or your basement walls.

    `;
  }

  if(waterLevelStatus === "High water level has been detected")
  { isUrgent = true;
    UserResponded = false;
    currentTime = Date.now();
    msg += `Please respond  with 'OK' to this message in the next 15 minutes to dismiss the alert or an warning call with be made.`;
    const voiceMsg = `
    Hello, This is a automated message by Flood, ${waterLevelStatus} in your basement. 
     Please check your basement sump pump for failure.
    `;
    setTimeout(callUser, 900000, voiceMsg);
  }
  sendAlert(msg)
  res.status(200);
})

app.listen(process.env.PORT || 8080, () => {
  console.log('Listening');
  

});

async function callUser(msg){
    if( UserResponded == false)
    {

        client.calls.create({
        twiml: `<Response><Say> ${msg} </Say></Response>`,
        to: myphone,
        from: twilioNumber
        });
        
    }
}
async function sendMessage(msg){
client.messages
  .create({
     body: msg,
     from: twilioNumber, // REPLACE ME WITH ENV VARIABLE
     to: myphone
   });
}
