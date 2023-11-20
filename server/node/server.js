const { Server } = require("socket.io");


var OnlineUser = [];
var Match = [];
var Match1v1v1 = [];
var inMatch = [];
//server = http.createServer().listen(8000);
const io = new Server(3200, {
  allowEIO3: true,
});

var run = function(socket){
  //console.log(socket);
  console.log(`Client ${socket.id} connected`);
  OnlineUser.push(socket);

  socket.on('message', function(msg){
      console.log(`Client ${socket.id} send: ${msg}`);
      if(msg == "match 1v1")
      {
        if(inMatch.indexOf(socket) == -1)
        {
          if (Match.indexOf(socket) == -1) 
          {
            console.log("pushing");
            Match.push(socket);
          }
  
          if (Match.length > 1)
          {
            rand = 0
            while(rand == Match.indexOf(socket)) rand = Math.floor(Math.random() * (Match.length+1));
            Match.splice(Match.indexOf(socket), 1)
            opponent = OnlineUser.indexOf(Match[rand]);
            sel = OnlineUser.indexOf(socket);
            Match.splice(rand, 1);
            socket.emit("message", "ready:1v1:move_first=0:opponent="+String(opponent));
            OnlineUser[opponent].emit("message", "ready:1v1:move_first=1:opponent="+String(sel));
            inMatch.push(socket, OnlineUser[opponent]);
          }
          else
          {
            socket.emit("message", "finding:1v1");
          }
          //console.log(Round);
        }
      }
      else if (msg.includes("inMatchWith"))
      {
        flag = true
        split_str = msg.split(":");
        send_id = split_str[0].split("=")[1];
        if(split_str[2].includes("run_away")) inMatch.splice(inMatch.indexOf(socket), 1);
        if(split_str[1].includes("move") || split_str[1].includes("end") || split_str[2].includes("pause_accept")
          || split_str[2].includes("resume")) flag = false;
        split_str.splice(0, 1);
        split_str = split_str.join(":")
        OnlineUser[send_id].emit("message", split_str);
        if(flag) socket.emit("message", "response")
        else socket.emit("message", split_str);

      }
      else if (msg.includes("Unmatch"))
      {
        Match.splice(Match.indexOf(socket));
        Match1v1v1.splice(Match1v1v1.indexOf(socket));
      }
      else if (msg.includes( "end" ))
      {
        console.log(inMatch.length);
        inMatch.splice(inMatch.indexOf(socket), 1);
      }
      else if (msg.includes("match 1v1v1"))
      {
        if(inMatch.indexOf(socket) == -1)
        {
          if (Match.indexOf(socket) == -1) 
          {
            console.log("pushing");
            Match.push(socket);
          }
  
          if (Match.length > 1)
          {
            rand = 0
            while(rand == Match.indexOf(socket)) rand = Math.floor(Math.random() * (Match.length+1));
            Match.splice(Match.indexOf(socket), 1)
            opponent = OnlineUser.indexOf(Match[rand]);
            sel = OnlineUser.indexOf(socket);
            Match.splice(rand, 1);
            socket.emit("message", "ready:1v1v1:move_order=0:opponent="+String(opponent));
            OnlineUser[opponent].emit("message", "ready:1v1v1:move_order=1:opponent="+String(sel));
            inMatch.push(socket, OnlineUser[opponent]);
          }
          else
          {
            socket.emit("message", "finding:1v1v1");
          }
          //console.log(Round);
        }
      }
      else if (msg.includes("1v1v1MatchWith"))
      {
        flag = true
        split_str = msg.split(":");
        send_id = split_str[0].split("=")[1];
        if(split_str[2].includes("run_away")) inMatch.splice(inMatch.indexOf(socket), 1);
        if(split_str[1].includes("move") || split_str[1].includes("end") || split_str[2].includes("pause_accept")
          || split_str[2].includes("resume")) flag = false;
        split_str.splice(0, 1);
        split_str = split_str.join(":")
        OnlineUser[send_id].emit("message", split_str);
        if(flag) socket.emit("message", "response")
        else socket.emit("message", split_str);
      console.log(split_str);
      }

  });

  socket.on('disconnect', ()=>{
    OnlineUser.splice(OnlineUser.indexOf(socket), 1);
    Match.splice(Match.indexOf(socket), 1);
    inMatch.splice(inMatch.indexOf(socket), 1);
    console.log(`Client ${socket.id} disconnected`)
  });


}

io.sockets.on('connection', run);