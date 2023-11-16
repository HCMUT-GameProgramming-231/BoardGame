const { Server } = require("socket.io");


var OnlineUser = [];
var Match = [];
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
  });

  socket.on('disconnect', ()=>{
    OnlineUser.splice(OnlineUser.indexOf(socket), 1);
    console.log(`Client ${socket.id} disconnected`)
  });


}

io.sockets.on('connection', run);