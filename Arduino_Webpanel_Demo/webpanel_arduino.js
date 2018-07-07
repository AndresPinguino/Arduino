/*	WebPanel

    Copyright 2010 Steven M. Marlow, Evexia LLC
	www.evexiallc.com
	info@evexiallc.com
	
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
	See the file named COPYING that is distributed with this program
*/

var wp; //webpanel object
var meta = "";	//variable to hold metadata message from client
var wpElementsLength = 0;	//variable to measure the number of elements in the document

var Analog0 = new Array();  //create the arrays for the analog readings here.
var Analog1 = new Array();  //they will be used in a "shift register" style
var Analog2 = new Array();  //to create a 100 element "strip chart"
 
window.onload = function ()	// Upon loading, the script will search the parent document for webpanel animation elements by ID                                 
{
	top.wpSend = wpSend;	//for sending webpanel msg from HTML parent if SVG is. For Chrome, 
							// launch with switch -allow-file-access-from-files if using localhost
	var wsURL="ws://192.168.1.77/";	//URL of the websocket server. 
									//Edit this address before serving to client, or leave it for demo mode on  single PC
									
	var wpElements = document.getElementsByTagName('*');	//get all tagged elements in the parent document
	
	var tempText = document.getElementById("textTemp");

	// Update and clean all the white spaces, etc., from webpanel objects

	for (i=0; i<=wpElements.length; i++)
	{ 
		if (wpElements[i])
		{
			var wpType = wpElements[i].getAttribute("wp:type");
			if (wpType)
			{
				var parent = wpElements[i].parentNode; //set parent node variable   
				var pAttr = "";  //parent attribute to modify
				
				//Clean the whitespace and "no Id" child nodes from webpanel objects
				for (j=0; j<wpElements[i].childNodes.length; j++)
				{
					var child = wpElements[i].childNodes[j];
					if(!child.id)
					{
						wpElements[i].removeChild(child);
						j--;
					}
				} 

				// Set ID attributes in nodes/elements that are parents of active (animated) child elements.  This is necessary because the SVG widgets are groups of elements,
				// and if the user makes multiple copies in a single document, the child node object IDs are renamed by inkscape.  This piece of script finds all of the webpanel
				// animated elements (needles, graph lines, etc.) and updates the parent object's webpanel attribute with the correct animated element object ID.
				// When an incoming message targeting a webpanel parent object is received, it's attribute containing the element ID for the moving child object will have the
				// correct ID.  This method was replaced in version 0.2 by using JSON strings and jquery for the wp object properties.   The use of jquery made it easier to 
				// find elements without having to use the getElementByID method.
               				
				switch(wpType)
				{
					case "ledLight":
					pAttr = "wp:ledLightId";
					break;
					
					case "textLed":
					pAttr = "wp:ledTextId";
					break;
					
					case "needle":
					pAttr = "wp:needleId";
					break;
					
					case "centerRotation":
					pAttr = "wp:centerId";
					break;

					case "graphLine":
					pAttr = "wp:graphLineId";
					break;

					case "graphArea":
					pAttr = "wp:graphAreaId";
					break;
         
					case "valueText":
					pAttr = "wp:valueTextId";
					break;

					case "progressBar":
					pAttr = "wp:progressBarId";
					break;

					case "off":
					pAttr = "wp:offId";
					break;

					case "on":
					pAttr = "wp:onId";
					break;

					case "textLabel":
					var textStyle = wpElements[i].getAttribute("style");
					wpElements[i].setAttribute("style", textStyle + ";-webkit-user-select:none;-moz-user-select:none"); //disable text selection
					break;
					
					case "meter":
					case "graph":
					case "led":
					case "progressHorizontal":
					case "progressVertical":
					AppendMeta(wpElements[i].id, wpType);
					pAttr = "";
					break;
					
					case "image":
					default:
					pAttr = "";
					break;                 
				}
				if (pAttr)
				parent.setAttribute(pAttr, wpElements[i].id);
			}
		}
	}
    
	if ("WebSocket" in window)
	{
		// open webpanel websocket connection
		wp = new WebSocket(wsURL);
		
		wp.onopen = function()
		{
			// Webpanel is connected. Send metadata description of top-level elements
			// wp.send("metadata;" + wpElementsLength + meta); NOT USED FOR ARDUINO DEMO
			// You can put something here is you want to signal the Arduino that the browser has connected
		};

		wp.onmessage = function(evt) 
		{ 
			// handle websocket message. update attributes or values of elements that match the name on incoming message
			var msgArray = evt.data.split(","); //split message by delimiter into a string array
			var indicator = document.getElementById(msgArray[0]);  //the first element in the message array is the ID of the object to update
			
			if (indicator)  //if an object by the name of the message exists, update its value or its attributes
			{
				//switch handling for value, atttribute, or Arduino Demo.  The Arduino case was added to allow special message processing
                //while leaving the rest of the original LabVIEW webpanel intact.  The Arduino
				
				switch(msgArray[1]) 				
				{
					case "value":  //The message is a value. Animate the element
					switch (indicator.getAttribute("wp:type")) //get the element's type to switch how to animate
					{
						//the webpanel type is meter. rotate the needle
						case "meter":
						var centerId = indicator.getAttribute("wp:centerId");
						var center = document.getElementById(centerId);
						var needleId = indicator.getAttribute("wp:needleId");
						var needle = document.getElementById(needleId);
						var valueTextId =  indicator.getAttribute("wp:valueTextId");
						var valueText = document.getElementById(valueTextId);
						var cx = center.getAttribute("cx");
						var cy = center.getAttribute("cy");
						var slope = parseFloat(indicator.getAttribute("wp:slope"));
						var offset = parseFloat(indicator.getAttribute("wp:offset"));
						var rotation = parseFloat(msgArray[2]) * slope + offset;
						needle.setAttribute("transform","rotate(" + rotation + "," + cx + "," + cy + ")");
						valueText.textContent = msgArray[2];
						break;
						
						//webpanel type is graph. update the line
						case "graph":
						var graphLineId = indicator.getAttribute("wp:graphLineId");
						var graphLine = document.getElementById(graphLineId);
						graphLine.setAttribute("d",msgArray[2]);
						break;

						//switch two-pic boolean view by swapping child order
						case "bool2pic":
						var boolValue = parseInt(msgArray[2]);
						var picOnId = indicator.getAttribute("wp:onId");
						var picOn = document.getElementById(picOnId);
						var picOffId = indicator.getAttribute("wp:offId");
						var picOff = document.getElementById(picOffId);
						indicator.appendChild(ledValue?picOn:picOff); //move child view to the end so it is on top
						indicator.setAttribute("wp:value", boolValue);
						break;
						
						//switch led by changing background light color and on/off text
						case "led":
						var ledValue = parseInt(msgArray[2]);
						var ledLightId = indicator.getAttribute("wp:ledLightId");
						var ledLight = document.getElementById(ledLightId);
						var ledOnColor = ledLight.getAttribute("wp:onColor");
						var ledOffColor = ledLight.getAttribute("wp:offColor");
						ledLight.style.fill = ledValue?ledOnColor:ledOffColor;
						var ledTextId = indicator.getAttribute("wp:ledTextId");
						var ledText = document.getElementById(ledTextId);
						var ledOnText = ledText.getAttribute("wp:onText");
						var ledOffText = ledText.getAttribute("wp:offText");
						ledText.textContent = ledValue?ledOnText:ledOffText;
						indicator.setAttribute("wp:value", ledValue);
						break;
						
						//update horizontal progress bar width
						case "progressHorizontal":
						var progressBarId = indicator.getAttribute("wp:progressBarId");
						var progressBar = document.getElementById(progressBarId);
						progressBar.setAttribute("width",msgArray[2]);
						break;
						
						//update vertical progress bar height
						case "progressVertical":
						var progressBarVid = indicator.getAttribute("wp:progressBarId");
						var progressBarV = document.getElementById(progressBarVid);
						progressBarV.setAttribute("height",msgArray[2]);
						progressBarV.setAttribute("y",(100 - parseInt(msgArray[2])));
						break;
						
						//update an image
						case "image":
						var imgSRC="data:image/png;base64," + msgArray[2];
						indicator.setAttribute("src", imgSRC);
						break;
						
						default:
						// unrecognized webpanel type. do nothing
						break;                 
					}
					
					case "attribute": 
					//the message is an attribute update.  TO DO: update scale ranges, etc., here. 
					break;
					
					case "Arduino":
					//This special case is has been added for the Arduino Demonstration.  It is the only case used by the sample sketch.  You can use the other types, but you have to format
					//the data to fit the sample SVG objects, or modify the object scales to fit the data.  In the original webpanel demo, all of the data scaling was done in LabVIEW
					//becuase the CompactRIO controller has a floating point processor.  You can also modify the cases above to scale the Arduinpo data using this special demo case as a //guide.
					
					//Convert the message strings to numbers
					var A0 = parseFloat(msgArray[2]);
					var A1 = parseFloat(msgArray[3]);
					var A2 = parseFloat(msgArray[4]);
					var D8 = parseInt(msgArray[5]);
					
					//webpanel type is graph. update the lines
					//indicator object variable is already set to wpGraph object in line 155
					//The graph must be in the SVG doc, or you must change the Arduino sketch to target another existing object.
					
					//Update the graph data buffers with the new values  Append to array if array size is less than 100.  Else FIFO shift the array and add new value
					var x = Analog0.length;
					if (x < 101) 
					{
						Analog0[x] = A0;
						Analog1[x] = A1;
						Analog2[x] = A2;
					}
					else
					{
						Analog0.shift();
						Analog1.shift();
						Analog2.shift();
						x = Analog0.length;
						Analog0[x] = A0;
						Analog1[x] = A1;
						Analog2[x] = A2;
					}
					
					//Get the wpGraph object properties
					var originX = parseFloat(indicator.getAttribute("wp:originX"));
					var originY = parseFloat(indicator.getAttribute("wp:originY"));
					var height = parseFloat(indicator.getAttribute("wp:graphHeight"));
					var width = parseFloat(indicator.getAttribute("wp:graphWidth"));
					var maxX = parseFloat(indicator.getAttribute("wp:scaleMaxX"));
					var minX = parseFloat(indicator.getAttribute("wp:scaleMinX"));
					var maxY = parseFloat(indicator.getAttribute("wp:scaleMaxY"));
					var minY = parseFloat(indicator.getAttribute("wp:scaleMinY"));
					var Line0 = document.getElementById("path0");
					var Line1 = document.getElementById("path1");
					var Line2 = document.getElementById("path2");
					
					var debugText = document.getElementById("textDebug");
					debugText.textContent = ""; //Analog2.length.toString();
					
					var Xscale = width/(maxX-minX);
					var X = originX;
					var Yscale = (height/(maxY-minY))*(-1);
					var Y0 = originY + Analog0[0]*Yscale;
					var Y1 = originY + Analog1[0]*Yscale;
					var Y2 = originY + Analog2[0]*Yscale;
					var dValue0 = "m " + X + "," + Y0;
					var dValue1 = "m " + X + "," + Y1;
					var dValue2 = "m " + X + "," + Y2;
					var pX = X;
					var pY0 = Y0;
					var pY1 = Y1;
					var pY2 = Y2;
					for (i=1; i<=Analog0.length; i++)
						{
							X = originX + i*Xscale;
							Y0 = originY + Analog0[i]*Yscale;
							Y1 = originY + Analog1[i]*Yscale;
							Y2 = originY + Analog2[i]*Yscale;
							var dX = X - pX;
							var dY0 = Y0 - pY0;
							var dY1 = Y1 - pY1;
							var dY2 = Y2 - pY2;
							dValue0 = dValue0 + " " + dX + "," + dY0;
							dValue1 = dValue1 + " " + dX + "," + dY1;
							dValue2 = dValue2 + " " + dX + "," + dY2;
							pX = X;
							pY0 = Y0;
							pY1 = Y1;
							pY2 = Y2;
						}
					Line0.setAttribute("d",dValue0);
					Line1.setAttribute("d",dValue1);
					Line2.setAttribute("d",dValue2);
						
					//Rotate the Dial Gage.
					var indicator = document.getElementById("wpMeter");
					var centerId = indicator.getAttribute("wp:centerId");
					var center = document.getElementById(centerId);
					var needleId = indicator.getAttribute("wp:needleId");
					var needle = document.getElementById(needleId);
					var valueTextId =  indicator.getAttribute("wp:valueTextId");
					var valueText = document.getElementById(valueTextId);
					var cx = center.getAttribute("cx");
					var cy = center.getAttribute("cy");
					var slope = parseFloat(indicator.getAttribute("wp:slope"));
					var offset = parseFloat(indicator.getAttribute("wp:offset"));
					var rotation = (A2/10.23) * slope + offset;
					needle.setAttribute("transform","rotate(" + rotation + "," + cx + "," + cy + ")");
					valueText.textContent = msgArray[4];
					
					//switch led by changing background light color and on/off text
					var indicator = document.getElementById("wpLED");
					var ledLightId = indicator.getAttribute("wp:ledLightId");
					var ledLight = document.getElementById(ledLightId);
					var ledOnColor = ledLight.getAttribute("wp:onColor");
					var ledOffColor = ledLight.getAttribute("wp:offColor");
					ledLight.style.fill = D8?ledOnColor:ledOffColor;
					var ledTextId = indicator.getAttribute("wp:ledTextId");
					var ledText = document.getElementById(ledTextId);
					var ledOnText = ledText.getAttribute("wp:onText");
					var ledOffText = ledText.getAttribute("wp:offText");
					ledText.textContent = D8?ledOnText:ledOffText;
					indicator.setAttribute("wp:value", msgArray[5]);
					
					//Display Grove temperature sensor on progress bar
					var indicator = document.getElementById("wpProgress");
					var resistance = (1023-A0)*10000/A0;   					//get the resistance of the sensor
					var R2 = Math.log(resistance/10000);
                    var temperature = 1/((R2/3975)+(1/298.15))-273.15; //convert to temperature via datasheet
					var progressBarId = indicator.getAttribute("wp:progressBarId");
					var progressBar = document.getElementById(progressBarId);
					var tempString = temperature.toFixed(2).toString();
					progressBar.setAttribute("width",tempString);
					var tempText = document.getElementById("textTemp");
					tempText.textContent = tempString;
					
					break;
					
					default:
					//unrecognized message type.  do nothing
					break;
					
				} 
			}
		};
		
		wp.onclose = function()
		{ 
			// websocket is closed.
			alert("Connection with " + wsURL + " was closed."); 
		};
	} 
	else
	{
		// The browser doesn't support WebSocket
		alert("WebSocket not supported by this Browser");
	}
}

function button(buttonId) //function for use by SVG buttons when they are clicked. execute using the onClick function of the SVG object
{
	var buttonObj = document.getElementById(buttonId); 
	var buttonValue = (!(parseInt(buttonObj.getAttribute("wp:value"))))?1:0;
	wp.send(buttonId + "|" + buttonValue);
	buttonObj.appendChild(buttonObj.childNodes[0]);
	var latch = parseInt(buttonObj.getAttribute("wp:latch"));
	if (latch)
	{
		var latchTime = parseInt(buttonObj.getAttribute("wp:latchTime"));
		var t=setTimeout("document.getElementById('" + buttonId + "').appendChild(document.getElementById('" + buttonId + "').childNodes[0]);",latchTime);
	}
	else
	{
	buttonObj.setAttribute("wp:value", buttonValue);
	}
}

function onOff(onOffId)
{
	var onOffObj = document.getElementById(onOffId); 
	var onOffValue = (!(parseInt(onOffObj.getAttribute("wp:value"))))?1:0;
	wp.send(onOffId + "|" + onOffValue);
	onOffObj.appendChild(onOffObj.childNodes[0]);
	onOffObj.setAttribute("wp:value", onOffValue);
}

function wpSend(txt)  //function for sending text from HTML document form objects when script is embedded
{
	wp.send(txt);
}

function AppendMeta(wpElement, wpType) //function to append metadata for top-level elements
{
	// Append meta data message with description of current element
	wpElementsLength++;
	meta = meta + ";" + wpElement + "," + wpType;
}