const accountSid =  process.env.TWILIO_ACCOUNT_SID;
const authToken =   process.env.TWILIO_AUTH_TOKEN;
const myphone = process.env.MY_PHONE_NUMBER;
const twilioNumber = process.env.TWILIO_NUMBER;
const {client, MessagingResponse} = require('twilio')(accountSid, authToken);
const express = require('express');

const app = express()
app.use(express.urlencoded({extended: true}));
app.use(express.json())
let isUrgent = false;
let UserResponded = true;
let currentTime;


app.get('/', (req, res) => {
 //TODO
  res.send(200)
})

app.post('/sms', (req, res) => {
    const twiml = new MessagingResponse();
    twiml.message('Alert dismissed , Thank you for confirming.');
    res.writeHead(200, {'Content-Type': 'text/xml'});
    res.end(twiml.toString());
    
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
     Water Level : ${waterLevelStatus} ✅
     Water Leaks : ${waterLeakStatus} ❌ \n

     Water is ${waterDistance} ft away from the floor of the basement at ${time} on ${Date.now()}.
     \n Please check your Sump pump for possible failure or your basement walls.

    `;
  }

  if(waterLevelStatus === "High water level has been detected")
  { isUrgent = true;
    UserResponded = false;
    currentTime = Date.now();
    msg += `\nPlease respond  with 'OK' to this message in the next 15 minutes to dismiss the alert or an warning call with be made.`;
    const voiceMsg = `
    Hello, This is a automated message by Flood, ${waterLevelStatus} in your basement. 
     Please check your basement sump pump for failure. Goodbye!
    `;
    setTimeout(callUser, 900000, voiceMsg);
  }
  sendMessage(msg);
  res.sendStatus(200)
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
