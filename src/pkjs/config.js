'use strict';
/* eslint-disable quotes */

module.exports = [
    {
        "type": "heading",
        "id": "main-heading",
        "defaultValue": "ARRL Logo Watch",
        "size": 1
    },

    {
        "type": "text",
        "defaultValue": "Use this configuration page to configure your watch!"
    },

         //****************
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "DATE FORMAT:"
            },
            {
                "type": "radiogroup",
                "messageKey": "DATE_FORMAT_KEY",
                "label": "",
                "defaultValue": "0",
                "options": [
                    { "label": "Jun 5, 2016", "value": "0" },
                    { "label": "5 Jun 2016",  "value": "1" }
                    
                ]
            },
       //**********************

            {
                "type": "section",
                "items": [
                    {
                        "type": "heading",
                        "defaultValue": "BLUETOOTH:"
                    },
                    {
                        "type": "toggle",
                        "messageKey": "BT_VIBRATE_KEY",
                        "label": "Vibrate on Loss of Bluetooth",
                        "defaultValue": false
                    }
                ]
            },
         
        //******************
            {
                "type": "section",
                "items": [
                    {
                        "type": "heading",
                        "defaultValue": "LOW BATTERY"
                    },
                    {
                        "type": "toggle",
                        "messageKey": "LOW_BATTERY_KEY",
                        "label": "Vibrate on Low Battery",
                        "defaultValue": false
                    }
                ]
            },
       
            //****************************
           
            // * * * 
            
            {
                "type": "heading",
                "defaultValue": ""
            },
            {
                "type": "submit",
                "defaultValue": "Save"
            },
            {
                "type": "text",
                "defaultValue": "ARRL Logo By WA1OUI V8.0, Logo © ARRL"
            }
        ]
    }
];