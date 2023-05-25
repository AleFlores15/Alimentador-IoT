const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");

// Elements for sensor readings
const ipElement = document.getElementById("ipe");
const hostElement = document.getElementById("host");
const statusElement = document.getElementById("status");
const ssidElement = document.getElementById("ssid");
const tempElement = document.getElementById("temp");
const humElement = document.getElementById("hum");
const presElement = document.getElementById("pres");
const ledElement = document.getElementById("led");
const BombaElement = document.getElementById("bomb");
const rssiElement = document.getElementById("rssi");
var dbPathLed;
var dbPathBomba;
var dbPathChart;

// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display = 'block';
    authBarElement.style.display ='block';
    userDetailsElement.style.display ='block';
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from database
    var uid = user.uid;
    console.log(uid);

    // Database paths (with user UID)
    var dbPathTemp = 'UsersData/' + uid.toString() + '/temperature';
    var dbPathHum = 'UsersData/' + uid.toString() + '/humidity';
    var dbPathPres = 'UsersData/' + uid.toString() + '/pressure';
    var dbPathPres = 'UsersData/' + uid.toString() + '/pressures';
    var dbPathip = 'UsersData/' + uid.toString() + '/direccionip';
    var dbPathrssi = 'UsersData/' + uid.toString() + '/rssi';
    var dbPathssid = 'UsersData/' + uid.toString() + '/ssid';
    var dbPathhost = 'UsersData/' + uid.toString() + '/host';
    var dbPathstatus = 'UsersData/' + uid.toString() + '/status';
    dbPathLed = 'UsersData/' + uid.toString() + '/led';
    dbPathBomba = 'UsersData/' + uid.toString() + '/bomba';
    dbPathChart = 'UsersData/' + uid.toString() + '/chart';
    // Database references
    var dbRefTemp = firebase.database().ref().child(dbPathTemp);
    var dbRefHum = firebase.database().ref().child(dbPathHum);
    var dbRefPres = firebase.database().ref().child(dbPathPres);
    var dbRefip = firebase.database().ref().child(dbPathip);
    var dbRefrssi = firebase.database().ref().child(dbPathrssi);
    var dbRefssid = firebase.database().ref().child(dbPathssid);
    var dbRefstatus = firebase.database().ref().child(dbPathstatus);
    var dbRefhost = firebase.database().ref().child(dbPathhost);
    var dbPathLed = firebase.database().ref().child(dbPathLed);
    var dbPathLed = firebase.database().ref().child(dbPathBomba);
    var dbRefChart = firebase.database().ref().child(dbPathChart);
    // Update page with new readings
    dbRefChart.on('value', snap => {

      dbRefChart.innerText = snap.val().toFixed(2);
      var x = (new Date()).getTime(),
      y= parseFloat(snap.val().toFixed(2));

         // y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
      } else {
        chartT.series[0].addPoint([x, y], true, false, true);
      }
    
    });

    dbRefhost.on('value', snap => {
      hostElement.innerText = snap.val().toString();
    });

    dbRefssid.on('value', snap => {
      ssidElement.innerText = snap.val().toString();
    });
    dbRefstatus.on('value', snap => {
      statusElement.innerText = snap.val().toString();
    });

    dbRefHum.on('value', snap => {
      humElement.innerText = snap.val().toFixed(2);
    });

    dbRefip.on('value', snap => {
      ipElement.innerText = snap.val().toString();
    });

    dbRefPres.on('value', snap => {
      presElement.innerText = snap.val().toFixed(2);
    });
    dbRefrssi.on('value', snap => {
      rssiElement.innerText = snap.val().toFixed(2);
    });

  // if user is logged out
  } else{
    // toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display ='none';
    userDetailsElement.style.display ='none';
    contentElement.style.display = 'none';
  }
}
function toggleLed() {
  console.log("Toggle");
  if (ledElement.checked) 
  {
    console.log("led ON");
    firebase.database().ref(dbPathLed).set("ON");
  }
  else{
    console.log("led OFF");
    firebase.database().ref(dbPathLed).set("OFF");
  }
}


function toggleLed1() {
  console.log("Toggle");
  if (BombaElement.checked) 
  {
    console.log("led ON");
    firebase.database().ref(dbPathBomba).set("ON");
  }
  else{
    console.log("led OFF");
    firebase.database().ref(dbPathBomba).set("OFF");
  }
}


/*setInterval(function ( ) {
 
      var x = (new Date()).getTime(),
      y=5;
         // y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
      } else {
        chartT.series[0].addPoint([x, y], true, false, true);
      }
    
 
 
}, 1000 ) ;*/