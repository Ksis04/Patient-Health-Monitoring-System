// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
//import { getAnalytics } from "firebase/analytics";
import { getDatabase } from "firebase/database";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBxIkX3AI-kcVgTyIrFQRxE0VSc900z_U0",
  authDomain: "health-report1.firebaseapp.com",
  databaseURL: "https://health-report1-default-rtdb.firebaseio.com",
  projectId: "health-report1",
  storageBucket: "health-report1.appspot.com",
  messagingSenderId: "897407788178",
  appId: "1:897407788178:web:00521376daf018f09e724b",
  measurementId: "G-3HCEQSX18T"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
// const analytics = getAnalytics(app);
const database = getDatabase(app);

export { database };
