const { Server } = require("socket.io");

//server = http.createServer().listen(8000);
const io = new Server(3200, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"],
    credentials: true,
  }, 
  allowEIO3:true,
});

var run = function(socket){
  console.log('connect....');

 
  //socket.send('Hello from SocketIO');

  socket.on('message', function(msg){
      console.log('Received: ' + msg);
      socket.emit('message', 'Hello from Socket.IO');
  });

}

io.sockets.on('connection', run);