namespace mesh {

  const int PORT = 5555 ;
  Scheduler s ;
  painlessMesh m ;
  
   
   void setup(void) {
    m.init(eemem::store.probe.mesh.wifiId,eemem::store.probe.mesh.wifiPwd,&s,PORT) ;
    m.onReceive(&message::receiveToBuffer) ;
   }

  void loop(void) {
    m.update() ;  
  }
   
}
