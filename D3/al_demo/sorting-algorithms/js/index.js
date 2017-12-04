  /*Life's Not Complete Without Art. (Expand Your Editor > ASCII Is Not Responsive ;)
  
       __,:,__       :,__               __,:       __,:,__
    ,ad88P`Y88ba,    `Y88ba,         ,ad88P'    ,ad88P`Y88ba,
   ad88888' `88888ba   `88888ba     ad88888'   ad88888' `88888ba
 ,d88888P'   `Y88888b,  `Y88888b, ,d88888P'  ,d88888P'   `Y88888b,
,d888P"'       `"Y888b,   `"Y888b,d888P"'   ,d888P"'       `"Y888b,
:(["               "]):       "]):(["       :(["               "]):
`Y88ba,         ,ad88P'    ,ad88P`Y88ba,    `Y88ba,         ,ad88P'
 `88888ba     ad88888'   ad88888' `88888ba   `88888ba     ad88888'
  `Y88888b, ,d88888P'  ,d88888P'   `Y88888b,  `Y88888b, ,d88888P'
    `"Y888b,d888P"'   ,d888P"'       `"Y888b,   `"Y888b,d888P"'
:,__    ``":"''    __,:"''    __,:,__    ``":,__    ``":"''    __,:
`Y88ba,         ,ad88P'    ,ad88P`Y88ba,    `Y88ba,         ,ad88P'
 `88888ba     ad88888'   ad88888' `88888ba   `88888ba     ad88888'
  `Y88888b, ,d88888P'  ,d88888P'   `Y88888b,  `Y88888b, ,d88888P'
    `"Y888b,d888P"'   ,d888P"'       `"Y888b,   `"Y888b,d888P"'   
        "]):(["       :(["               "]):       "]):(["       
     ,ad88P`Y88ba,    `Y88ba,         ,ad88P'    ,ad88P`Y88ba,
   ad88888' `88888ba   `88888ba     ad88888'   ad88888' `88888ba
 ,d88888P'   `Y88888b,  `Y88888b, ,d88888P'   d88888P'   `Y88888b,
,d888P"'       `"Y888b,   `"Y888b,d888P"'   ,d888P"'       `"Y888b,
:"''    __,:,__    ``":,__    ``":"''    __,:"''    __,:,__    ``":
     ,ad88P`Y88ba,    `Y88ba,         ,ad88P'    ,ad88P`Y88ba,
   ad88888' `88888ba   `88888ba     ad88888'   ad88888' `88888ba
 ,d88888P'   `Y88888b,  `Y88888b, ,d88888P'  ,d88888P'   `Y88888b,
,d888P"'       `"Y888b,   `"Y888b,d888P"'   ,d888P"'       `"Y888b,
:(["               "]):       "]):(["       :(["               "]):
`Y88ba,         ,ad88P'    ,ad88P`Y88ba,    `Y88ba,         ,ad88P'
 `88888ba     ad88888'   ad88888' `88888ba   `88888ba     ad88888'
  `Y88888b, ,d88888P'  ,d88888P'   `Y88888b,  `Y88888b, ,d88888P'
    `"Y888b,d888P"'   ,d888P"'       `"Y888b,   `"Y888b,d888P"'
        ``":"''       :TMR               ``":       ``":"''
  
  */



   var borderColor = "#C13100";  
   var barColor = "#C13100";
   var finishedBarColor = "#CC6600";
   var movingBarColor = "#FFFBD0";
   var movingBarOutlineColor = "#FFFBD0";
   var boxColor = "#FF9900";
   var multiBoxColor = "#FF9900";
   var maxColor = "#4F2412";
   
   var barGap = 10;
   
   var IDLE = 0;     // values for the "state" variable.
   var STARTING = 1;
   var RUN = 2;
   var STEPPING = 3;
   var PAUSED = 4;
   
   var state = IDLE;
   
   var g;  // graphic context for drawing on the canvas.
   var width, height;  // width and height of the canvas.
   
   var barWidth, barHeight, minBarHeight, barIncrement; // measurements used for drawing.
   var leftOffset, firstRow_y, secondRow_y, textAscent;

   var method;  // The sorting method that is being used, coded as 1,2,3,4,5; controlled by a select element.
   
   var fast = false;  //Set to true for speed;  the value is controlled by a checkbox.
   
   var item = new Array();  // a 33-element array containing the numbers to be sorted in positions 1 to 16.
   // item[0] holds the value of temp.  positions 17 - 33 are used in MergeSort. A value of -1 in this array means that no item is present.  When an item is in its final position, 100 is added to the value as a signal that the item should be drawn in black.
   
   var tempOn = false;     // Variables that control the extra stuff that is sometimes drawn, in addition to items.
   var mergeBox = [ -1, -1, -1 ];;
   var multiBoxLoc = { x: -1, y: -1 };
   var movingItemLoc = { x: -1, y: -1 };
   var maxLoc, hiLoc, loLoc, box1Loc, box2Loc, movingItem;
   
   var copyCt; // Number of copies done so far in the current sort.
   var compCt; // Number of comparisons done so far in the current sort.
   
   var timeout = null;  // When not null, indicates a pending timeout (so it can be cleared to stop the animation).
   
   function say1(message) { // put the message in the paragraph with id "message1"
     $("#message1").html(message);
   }

   function say2(message) {  // put the message in the paragraph with id "message2", unless running at "fast" speed
     if (!fast || state != RUN || message == "")
        $("#message2").html(message);
   }

   function stopRunning() {  // does any pending actions in the action queue (with no delay) and cancels any timeout.
      while (actionQueue.length > 0) {
         doAction(actionQueue.shift());
      }
      if (timeout) {
         clearTimeout(timeout);
         timeout = null;
      }
   }
   
   function setState(newState) {  // called whenever the state changes; sets enabled/disabled status of various elements.
      state = newState;
      $("#runBtn").attr("disabled", state == RUN || state == IDLE || state == STEPPING);
      $("#pauseBtn").attr("disabled", state != RUN);
      $("#stepBtn").attr("disabled", state == RUN || state == IDLE || state == STEPPING);
      $("#sortSelect").attr("disabled", state == RUN || state == PAUSED || state == STEPPING);
   }

   function newSort() {  // Set up to get ready for a new sort by storing items in random array positions, etc.
      stopRunning();
      setState(STARTING);
      valid = false;
      maxLoc = -1;
      hiLoc = -1;
      loLoc = -1;
      box1Loc = -1;
      box2Loc = -1;
      multiBoxLoc.x = -1;
      mergeBox[0] = [ -1, -1, -1 ];
      movingItem = -1;
      tempOn = false;
      for (var i = 1; i <= 16; i++)
         item[i] = i;
      for (var i = 16; i >= 2; i--) {
         var j = 1 + Math.floor(Math.random()*i);
         var temp = item[i];
         item[i] = item[j];
         item[j] = temp;
      }
      item[0] = -1;
      for (var i = 17; i < 33; i++)
         item[i] = -1;
      $("#compCt").html("0");
      $("#moveCt").html("0");
      compCt = 0;
      copyCt = 0;
      valid = false;
      say1("Click \"Run\" or \"Step\" to begin sorting. Check \"Fast\" to speed up sort process.");
      say2("");
      draw();
    }
    
    //-------------------------------- Drawing ------------------------------------------
    
   function putItem(i) {  // draws item i from the array item[]; if item[i] is -1, nothing is drawn.
      var h = item[i];
      if (h == -1)
         return;
      var x,y,ht;
      if (h > 16) { 
         ht = (h-100)*barIncrement + minBarHeight;
         g.fillStyle = finishedBarColor;
      }
      else {
         ht = h*barIncrement + minBarHeight;
         g.fillStyle = barColor;
      }
      if (i == 0) {
         x = leftOffset + ((barWidth+barGap)*15)/2;
         y = secondRow_y - ht;
      }
      else if (i < 17) {
         x = leftOffset + (i-1)*(barWidth+barGap);
         y = firstRow_y - ht;
      }
      else {
         x = leftOffset + (i-17)*(barWidth+barGap);
         y = secondRow_y - ht;
      }
      try {
         g.fillRect(x,y,barWidth,ht);
         g.strokeStyle = finishedBarColor;
         g.strokeRect(x,y,barWidth,ht);
      }
      catch (e) { // (Got an error during development when item[i] was undefined.  Shouldn't happen again. :-)
         if (timeout != null)
            timeout.cancel();
         setState(IDLE);
         alert("Internal error while drawing!!??");
      }
   }
   
   function drawMovingItem() { // Draws an item that is being moved to animate the copying of an item from one place to another.
      var ht = movingItem*barIncrement + minBarHeight;
      g.fillStyle = movingBarColor;
      g.fillRect(movingItemLoc.x,movingItemLoc.y-ht,barWidth,ht);
      g.strokeColor = movingBarOutlineColor;
      g.strokeRect(movingItemLoc.x,movingItemLoc.y-ht,barWidth,ht);
   }
   
   function drawMax() { // Writes "Max" under one of the items, with an arrow pointing to the item.
      var sw = 30;  // (guess at string width)
      var x = leftOffset + (maxLoc-1)*(barWidth+barGap) + barWidth/2;
      var y = firstRow_y + 38 + textAscent;
      g.fillStyle = maxColor;
      g.fillText("Max",x-sw/2,y+textAscent);
      g.strokeStyle = maxColor;
      g.beginPath();
      g.moveTo(x,y);
      g.lineTo(x,y-29);
      g.moveTo(x,y-29);
      g.lineTo(x+6,y-24);
      g.moveTo(x,y-29);
      g.lineTo(x-6,y-24);
      g.stroke();
   }
   
   function drawBox(boxLoc) { // draws a box around one of the items (indicated by boxLoc)
      var x,y;
      if (boxLoc == 0) {
         x = leftOffset + ((barWidth+barGap)*15)/2;
         y = secondRow_y;
      }
      else if (boxLoc < 17) {
         x = leftOffset + (boxLoc-1)*(barWidth+barGap);
         y = firstRow_y;
      }
      else {
         x = leftOffset + (boxLoc-17)*(barWidth+barGap);
         y = secondRow_y;
      }
      g.strokeStyle = boxColor;
      g.strokeRect(x-2,y-barHeight-2,barWidth+4,barHeight+4);
   }
   
   function drawMultiBox() {  // draws a box around items number multiBoxLoc.x through multiBoxLoc.y
      var x,y,wd;
      if (multiBoxLoc.x < 17) {
         y = firstRow_y;
         x = leftOffset + (multiBoxLoc.x-1) * (barWidth + barGap);
      }
      else {
         y = secondRow_y;
         x = leftOffset + (multiBoxLoc.x-17) * (barWidth + barGap);
      }
      wd = (multiBoxLoc.y - multiBoxLoc.x)*(barGap + barWidth) + barWidth;
      g.strokeStyle = multiBoxColor;
      g.strokeRect(x-4,y-barHeight-4,wd+8,barHeight+8);
   }
   
   function drawMergeListBoxes() { // Draws a pair of boxes around lists that are being merged in MergeSort
      var x,y,wd1,wd2;
      y = firstRow_y;
      x = leftOffset + (mergeBox[0]-1) * (barWidth + barGap);
      wd1 = (mergeBox[1] - mergeBox[0])*(barGap + barWidth) + barWidth;
      wd2 = (mergeBox[2] - mergeBox[0])*(barGap + barWidth) + barWidth;
      g.strokeStyle = multiBoxColor;
      g.strokeRect(x-4,y-barHeight-4,wd1+8,barHeight+8);
      g.strokeRect(x-4,y-barHeight-4,wd2+8,barHeight+8);
   }
   
   function draw() {  // Completely redraws the canvas to show the current state.
       g.clearRect(0,0,width,height);
       g.strokeStyle = borderColor;
       g.strokeRect(0,0,width,height);
       g.strokeRect(1,1,width-2,height-2);
       for (var i = 1; i <= 16; i++)
          putItem(i);
       g.fillStyle = borderColor;
       for (var i = 1; i <= 16; i++) {
          var sw = (i<10)? 6 : 12;
          g.fillText("" + i,leftOffset+(i-1)*(barWidth+barGap)+(barWidth-sw)/2,firstRow_y+6+textAscent);
       }
       for (var i = 17; i <= 32; i++)
          putItem(i);
      if (tempOn) {
         g.fillStyle = borderColor;
         var sw = 40;
         g.fillText("Temp",leftOffset + (16*barWidth+15*barGap - sw)/2, secondRow_y + 5 + textAscent);
         putItem(0);
      }
      if (maxLoc >= 0)
         drawMax();
      if (box1Loc >= 0) 
         drawBox(box1Loc);
      if (box2Loc >= 0)
         drawBox(box2Loc);
      if (multiBoxLoc.x > 0)
         drawMultiBox();
      if (mergeBox[0] > 0)
         drawMergeListBoxes();
      if (movingItem >= 0)
         drawMovingItem();
   }
   
   // ---------------------------- Stepping through the sorts ------------------------------
   
   var actionQueue = new Array(); // A queue of pending actions for implementing some aspects of the animation.
   
   var done = false;  // state variables for scripting the various sorting algorithms.
   var i, j, k;
   var hi, lo;
   var stack = new Array();
   var stackCt;
   var sortLength, end_i, end_j;
   var valid = false;  // false when a sort is just ready to start; set to true when the first step is taken.

  function copyItem(toItem, fromItem) {  // copy an item from one place to another (actually just enqueue actions)
      if (fast) { // enqueue a single copy action when the "fast" checkbox is selected.
         actionQueue.push( { action: "copy", from: fromItem, to: toItem, delay: 200 } );
      }
      else {  // enqueue a series of actions that move the item gradually from old position to new.
         var x1, y1, x2, y2;
         if (toItem == 0) {
            x2 = leftOffset + ((barWidth+barGap)*15)/2;
            y2 = secondRow_y;
         }
         else if (toItem < 17) {
            x2 = leftOffset + (toItem-1)*(barWidth+barGap);
            y2 = firstRow_y;
         }
         else {
            x2 = leftOffset + (toItem-17)*(barWidth+barGap);
            y2 = secondRow_y;
         }
         if (fromItem == 0) {
            x1 = leftOffset + ((barWidth+barGap)*15)/2;
            y1 = secondRow_y;
         }
         else if (fromItem < 17) {
            x1 = leftOffset + (fromItem-1)*(barWidth+barGap);
            y1 = firstRow_y;
         }
         else {
            x1 = leftOffset + (fromItem-17)*(barWidth+barGap);
            y1 = secondRow_y;
         }
         var dist = Math.round(Math.sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ));
         var ct = Math.round(dist / 3);
         if (ct > 25)
           ct = 25;
         else if (ct < 6)
           ct = 6;
         actionQueue.push( { action: "startmove", from: fromItem, x: x1, y:y1, delay: 100 } );
         for (var i = 0; i <= ct; i++) {
            actionQueue.push( { action: "move", x: x1 + Math.round(((x2-x1)*i)/ct), y: y1 + Math.round(((y2-y1)*i)/ct), delay: 25 } );
         }
         actionQueue.push( { action: "donemove", to: toItem, delay: 200 } );
      }
   }
   
   function swapItems (a, b) { // swaps two items; actually just enqueues actions
     copyItem(0, a);
     copyItem(a, b);
     copyItem(b, 0);
   }

   function greaterThan(itemA, itemB) {  // test if one item is greater than another; boxes are shown around the two items.
     compCt++;
     $("#compCt").html("" + compCt);
     putBoxes(itemA,itemB);
     return (item[itemA] > item[itemB]);
   }
   
   function putBoxes(itemA,itemB) {  // show boxes around two items
      box1Loc = itemA;
      box2Loc = itemB;
   }
     
   function scriptSetup() {  // The first step in a sort
      method = $("#sortSelect").val();
      say2(""); 
      switch (method) {
       case "1":  {
         j = 16;
         i = 1;
         say1("Phase 1:  largest item \"bubbles\" up to position 16");
         tempOn = true;
         break;
        }
       case "2":  { 
         j = 16;
         i = 2;
         say1("Phase 1:  Find the largest item and swap it with item 16");
         say2("Item 1 is the largest item seen so far during this phase");
         maxLoc = 1;
         tempOn = true;
         break;
        }
       case "3":  {
         j = 0;
         multiBoxLoc.x = 1;
         multiBoxLoc.y = 1;
         say1("The sublist in the box -- just item 1 for now -- is correctly sorted");
         break;
        }
       case "4":  {
         sortLength = 1;
         i = 1;
         end_i = 1;
         j = 2;
         end_j = 2;
         k = 17;
         lo = 0;
         hi = 1;
         say1("Phase 1:  Merge lists of length 1 into lists of length 2");
         say2("First, merge item 1 with item 2.");
         multiBoxLoc.x = 17;
         multiBoxLoc.y = 18;
         mergeBox = [ 1, 1, 2 ];
         break;
        }
       case "5":  {
         stackCt = 0;
         hi = 16;
         lo = 1;
         k = 0;
         i = 1;     // i and j are starting valuse for lo and hi
         j = 16;
         say1("Apply \"QuickSortStep\" to items 1 through 16.");
         say2("The range of possible final positions for item 1 is boxed.");
         multiBoxLoc.x = 1;
         multiBoxLoc.y = 16;
         tempOn = true;
         break;
        }
      }
    }
    
    
    function scriptStep() {  // Do one step in a sort.  
        switch (method) {
          case "1": // bubble sort
           if (i==j)  {
            say2("");
             putBoxes(-1, -1);
             if (j==2)  {
               say1("The sort is finished.");
               done = true;
               tempOn = false;
               item[1] = 100+item[1];
             }
             else {
               j = j - 1;
               i = 1;
               say1("Phase " + (17 - j) + ":  next largest item bubbles up to position " + j);
             }
            }
           else {
             if (greaterThan(i, i + 1))  {
               say2("Is item " + i + " bigger than item " + (i + 1) + "?  Yes, so swap them.");
               swapItems(i, i + 1);
             }
             else {
               say2("Is item " + i + " bigger than item " + (i + 1) + "?  No, so don't swap them.");
             }
             i = i + 1;
             if (i==j)  {
               actionQueue.push( { action: "finishItem", itemNum: j, delay: 100 } );
             }
            } // end case 1
            break;
          case "2":  // selection sort
           if (j==1)  {
             say1("The sort is finished.");
             say2("");
             done = true;
             item[1] = 100+item[1];
             tempOn = false;
            }
           else if (i == -1)  {
             say1("Phase " + (17 - j) + ":   Find the next largest item and move it to position " + j);
             say2("Item 1 is the largest item seen so far during this phase");
             i = 2;
             maxLoc = 1;
            }
           else if (i > j)  {
             putBoxes(-1, -1);
             k = maxLoc;
             actionQueue.push( {action: "maxoff", delay: 0} );
             if (k==j) 
               say2("Item " + j + " is already in its correct location.");
             else {
                if (j==2) 
                  say2("Swap item 2 with item 1");
                else
                  say2("Swap item " + j +  " with maximum among items 1 through " + (j - 1));
                swapItems(k, j);
             }
             actionQueue.push( { action: "finishItem", itemNum: j, delay: 100 } );
             j = j - 1;
             i = -1;
            }
           else if (greaterThan(i, maxLoc))  {
             say2("Item " +  i + " is bigger than item " +  maxLoc + ", so item " +  i + " is now the max seen.");
             maxLoc = i;
             i = i + 1;
            }
           else {
             say2("Item " +  i + " is smaller than item " + maxLoc + ", so item " + maxLoc + " is still the max seen.");
             i = i + 1;
            } // end case 2
            break;
          case "3": // insertions sort
           if (j==0)  {
             say1("Phase 1: Insert item 2 into its correct position in the sorted list.");
             say2("Copy item 2 to Temp.");
             copyItem(0, 2);
             j = 2;
             i = 1;
             tempOn = true;
            }
           else if (j==17)  {
             multiBoxLoc.x = -1;
             multiBoxLoc.y -1;
             for (var x = 1; x <= 16; x++)
                item[x] += 100;
             say1("The sort is finished.");
             done = true;
             say2("");
             tempOn = false;
            }
           else if (i==0)  {
             say2("Temp is smaller than all items in the sorted list; copy it to position 1.");
             copyItem(1, 0);
             i = -1;
            }
           else if (i == -1)  {
             putBoxes(-1, -1);
             say1("Items 1 through " + j + " now form a sorted list.");
            say2("");
            multiBoxLoc.x = 1;
            multiBoxLoc.y = j;
             j = j + 1;
             i = -2;
            }
           else if (i == -2)  {
             say1("Phase " + (j - 1) + ": Insert item " + j + "  into its correct position in the sorted list.");
             say2("Copy item " + j + " to Temp.");
             copyItem(0, j);
             i = j - 1;
            }
           else if (greaterThan(i, 0))  {
             say2("Is item " + i + " bigger than Temp?  Yes, so move it up to position " + (i + 1));
             copyItem(i + 1, i);
             i = i - 1;
            }
           else {
             say2("Is item " +  i + " bigger than Temp?  No, so Temp belongs in position " + (i + 1));
             copyItem(i + 1, 0);
             i = -1;
            }  // end case 3
            break;
          case "4": // merge sort
           if ((lo==1) && (sortLength==8))  {
             for (var x = 1; x <= 16; x++)
                item[x] += 100;
             say1("The sort is finished.");
             say2("");
             done = true;
            }
           else if (lo==1)  {
             hi = hi + 1;
             sortLength = sortLength * 2;
             say1("Phase " + hi + ":  Merge lists of length " + sortLength + " into lists of length " + (sortLength * 2));
             k = 17;
             i = 1;
             j = sortLength + 1;
             end_i = i + sortLength - 1;
             end_j = j + sortLength - 1;
             say2("First, merge items " + i + " through " + end_i + " with items " + j + " through " + end_j);
             multiBoxLoc.x = i + 16;
             multiBoxLoc.y = end_j + 16;
             mergeBox = [ i, end_i, end_j ];
             lo = 0;
            }
           else if ((end_i < i) && (end_j < j))  {
             if (k==33)  {
               multiBoxLoc.x = -1;
               multiBoxLoc.y = -1;
               mergeBox = [ -1, -1, -1 ];
               say2("Copy merged items back to original list.");
               for (var n = 1; n < 17; n++) {
                 actionQueue.push( { action: "copy", from: n + 16, to: n, delay: 100 } );
               }
               lo = 1;
               }
             else {
               end_i = end_i + 2 * sortLength;
               end_j = end_j + 2 * sortLength;
               j = end_i + 1;
               i = j - sortLength;
               if (sortLength == 1) 
                 say2("Next, merge item " + i + " with item " + j);
               else
                 say2("Next, merge items " + i + " through " + end_i + " with items " + j + " through " + end_j);
               multiBoxLoc.x = i + 16;
               multiBoxLoc.y = end_j + 16;
               mergeBox = [ i, end_i, end_j ];
             }
            }
           else if (end_i < i)  {
             putBoxes(-1, -1);
             say2("List 1 is empty; move item " +  j + " to the merged list.");
             copyItem(k, j);
             j = j + 1;
             k = k + 1;
            }
           else if (end_j < j)  {
             putBoxes(-1, -1);
             say2("List 2 is empty; move item " +  i + " to the merged list.");
             copyItem(k, i);
             i = i + 1;
             k = k + 1;
            }
           else if (greaterThan(i, j))  {
             say2("Is item " +  j + " smaller than item " + i +  "?  Yes, so move item " + j + " to merged list");
             copyItem(k, j);
             j = j + 1;
             k = k + 1;
            }
           else {
             say2("Is item " + j + " smaller than item " + i + "?  No, so move item " + i + " to merged list");
             copyItem(k, i);
             i = i + 1;
             k = k + 1;
            }  // end case 4      
            break;
          case "5": // quicksort
           if (k==0)  {
             if (hi==lo)  {
               say2("There is only one item in the range; it is already in its final position.");
               item[hi] = 100+item[hi];
               multiBoxLoc.x = -1;
               multiBoxLoc.y = -1;
               k = 1;
             }
             else {
               say2("Copy item " + lo + " to Temp");
               copyItem(0, lo);
               k = -1;
             }
            }
           else if (k==1)  {
             if (stackCt==0)  {
               say1("The sort is finished.");
               say2("");
               tempOn = false;
               done = true;
             }
             else {
               hi = stack[stackCt];
               lo = stack[stackCt - 1];
               j = hi;
               i = lo;
               stackCt = stackCt - 2;
               say1("Apply \"QuickSortStep\" to items " + lo + " through " + hi);
               say2("The range of possible final positions for item " + lo + " is boxed");
               multiBoxLoc.x = lo;
               multiBoxLoc.y = hi;
               k = 0;
             }
            }
           else if (k==2)  {
             say2("Item " + hi + " is in final position; smaller items below and bigger items above");
             multiBoxLoc.x = -1;
             multiBoxLoc.y = -1;
             item[hi] = 100+item[hi];
             if (hi < j)  {
               stack[stackCt + 1] = hi + 1;
               stack[stackCt + 2] = j;
               stackCt = stackCt + 2;
             }
             if (hi > i)  {
               stack[stackCt + 1] = i;
               stack[stackCt + 2] = hi - 1;
               stackCt = stackCt + 2;
             }
             k = 1;
            }
           else if (hi==lo)  {
             putBoxes(-1, -1);
             say2("Only one possible position left for Temp; copy Temp to position "  + hi);
             copyItem(hi, 0);
             k = 2;
            }
           else if (item[lo]==-1)  {
             if (greaterThan(0, hi))  {
               say2("Item " + hi + " is smaller than Temp, so move it; Temp will end up above it");
               copyItem(lo, hi);
               lo = lo + 1;
                multiBoxLoc.x = lo;
               multiBoxLoc.y = hi;
             }
             else {
               say2("Item " + hi + " is bigger than Temp, so Temp will end up below it");
               hi = hi - 1;
                multiBoxLoc.x = lo;
               multiBoxLoc.y = hi;
             }
            }
           else if (item[hi]==-1)  {
             if (greaterThan(lo, 0))  {
               say2("Item " +  lo + " is bigger than Temp, so move it; Temp will end up below it");
               copyItem(hi, lo);
               hi = hi - 1;
                multiBoxLoc.x = lo;
               multiBoxLoc.y = hi;
             }
             else {
               say2("Item " + lo + " is smaller than Temp, so Temp will end up above it");
               lo = lo + 1;
                multiBoxLoc.x = lo;
               multiBoxLoc.y = hi;
             }
            }  // end case 5
         } // end switch
       }  // end scriptStep()

   function doAction(what) {  // perform one action from the action queue; actions are encoded as objects.
      switch (what.action) {
      case "copy": 
          item[what.to] = item[what.from];
          item[what.from] = -1;
          copyCt++;
          $("#moveCt").html("" + copyCt);
          break;
      case "startmove": //alert("start move " + what.from);
          movingItem = item[what.from];
          item[what.from] = -1;
          movingItemLoc.x = what.x;
          movingItemLoc.y = what.y;
          break;
      case "move":
          movingItemLoc.x = what.x;
          movingItemLoc.y = what.y;
          break;
      case "donemove":  //alert("end move " + what.to + "," + movingItem);
          item[what.to] = movingItem;
          movingItem = -1;
          copyCt++;
          $("#moveCt").html("" + copyCt);
          break;
       case "finishItem":
          item[what.itemNum] += 100;
          break;
       case "maxoff":
          maxLoc = -1;
          break;
       }
   }
    
   function frame() {  // do one frame of the animation; set timeout for next frame if appropriate.
      timeout = null;
      fast = $("#fastCheckbox").attr("checked");
      if (actionQueue.length > 0) {
         var what;
         do {
            what = actionQueue.shift();
            doAction(what);
         } while (actionQueue.length > 0 && what.delay == 0);
         timeout = setTimeout(frame,Math.max(5,what.delay));
      }
      else {
         if (!valid){ 
            scriptSetup();
            valid = true;
            done = false;
            if (state == RUN)
               timeout = setTimeout(frame, fast? 100 : 1000);
         }
         else { 
            scriptStep();
            if (!done && state == RUN)
               timeout = setTimeout(frame, fast? 100 : 1000);
         }
         if (done)
            setState(IDLE);
         else if (state == STEPPING)
            setState(PAUSED);
      }
      if (done && actionQueue.length == 0)
         setState(IDLE);
      else if (state == STEPPING && actionQueue.length == 0)
         setState(PAUSED);
      draw();
   }
   
    
    // ---------------------------- Control and Initialization -------------------------------
    
    function doRun() { // handler for "Run" button
       if (state == RUN || state == IDLE || state == STEPPING)
          return; // won't happen if button enable/disable is functioning
       setState(RUN);
       frame();
    }
    
    function doStep() { // handler for "Step" button
       if (state == RUN || state == IDLE || state == STEPPING)
          return; // won't happen if button enable/disable is functioning
       setState(STEPPING);
       frame();
    }
    
    function doPause() { // handler for "Pause" button
       if (state != RUN)
          return; // won't happen if button enable/disable is functioning
       stopRunning();
       setState(PAUSED);
       draw();
    }
    
    function doNew() { // handler for "New" button
       newSort();
    }
   
   $(function() { // initialization; called by jQuery when the document is ready.
      var canvas = document.getElementById("sortcanvas"); // A reference to the canvas element.
      if (! canvas ||  ! canvas.getContext ) {
             // This browser does not support canvas since the canvas
             // element has no getContext function 
         $("#message1").html("Sorry, your browser doesn't support the canvas element.");
         return;
      }
      g = canvas.getContext("2d");
      if ( ! g.fillText ) {  // eliminate missing function error in excanvas.js for IE 8 and earlier
         g.fillText = function() { };
      }
      g.font = "bold 11pt sans-serif";
      width = canvas.width;
      height = canvas.height;
      var x = (width - 20 + barGap)/16;
      barWidth =  x - barGap;
      textAscent = 15;
      leftOffset = (width - 16*barWidth - 15*barGap)/2;
      barHeight = (height - 40 - 2*textAscent) / 2;
      barIncrement = (barHeight-3)/17;
      minBarHeight = barHeight - 17*barIncrement;
      firstRow_y = barHeight + 10;
      secondRow_y = 2*barHeight + 25 + textAscent;
      $("#sortSelect").val(2);
      $("#fastCheckbox").attr("checked", false);
      $("#runBtn").click(doRun);
      $("#stepBtn").click(doStep);
      $("#pauseBtn").click(doPause);
      $("#newBtn").click(doNew);
      $("#fastCheckbox").change(function() { if ($("#fastCheckbox").attr("checked")) say2(""); } );
      newSort();
   });