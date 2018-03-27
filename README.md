## NS3 Simulation

hybrid-virtual-routing, for VLC-RF Backended Wi-Fi Relay Network

### Framework

* describe the network from-top-to-bottom in JSON format, self-recursive and iterable

* Demo

  ```mermaid
  graph TB
  	subgraph GroupB
  		A1[Router1]-.VLC.->B1_0[Relay_1_0]
  		B1_0-.WiFi.->A1
  		A1-.VLC.->B1_1[Relay_1_1]
  		B1_1-.WiFi.->A1
  	end
  	subgraph GroupA
  		A0[Router0]-.VLC.->B0_0[Relay_0_0]
  		B0_0-.WiFi.->A0
  		A0-.VLC.->B0_1[Relay_0_1]
  		B0_1-.WiFi.->A0
  	end
  	B0_0---C0[UE Group]
  	B0_1---C1[UE Group]
  	B1_0---C2[UE Group]
  	B1_1---C3[UE Group]
  ```

### Howto

1. build ns3 following official course;

2. `ln -s <this-repo-full-path> <your-ns3-full-path>/examples/main`

3. add following code in your `~/.zshrc` or `~/.bashrc` :

   ```shell
   # Waf section
   WAF_PATH=<your-ns3-full-path>
   WAF_EDITOR=<your-prefer-editor>
   export PATH=$WAF_PATH:$PATH

   function wafls() {
   	ls $WAF_PATH/$1 ${${*}#${1}}
   }

   function wafcd() {
   	cd $WAF_PATH/$1 ${${*}#${1}}
   }

   function wafed() {
   	$WAF_EDITOR $WAF_PATH/$1 ${${*}#${1}}
   }

   function wafbuild() {
   	local pwd_tmp=$PWD
   	cd $WAF_PATH &&	./waf
   	cd $pwd_tmp
   }

   function wafrun() {
   	local pwd_tmp=$PWD
   	cd $WAF_PATH &&	waf --run $*
   	cd $pwd_tmp
   }

   function wafpcap_mv() {
   	rm -rf ./$1-\*.pcap && mv -f $WAF_PATH/$1-*.pcap ./$1-pcap/
   }
   ```

   `source` the configuration file

4. `wafrun main` to run the main simulation

### Todo
- [ ] Network JSON Parse
- [ ] Static Routing Strategy
- [ ] Performance Analysis Callback
- [ ] Wi-Fi Collision Container
