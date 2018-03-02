## NS3 Simulation

hybrid-virtual-routing, for VLC-RF Backend Wi-Fi Relay Network

### Howto


1. build ns3 following official course;

2. `ln -s <this-repo-full-path> <your-ns3-full-path>/examples/main`

3. add following code in your `~/.zshrc` or `~/.bashrc` :

   ```shell
   # Waf section
   WAF_PATH=<your-ns3-full-path>
   export PATH=$WAF_PATH:$PATH
   _editor_check() { echo `whereis $1 | awk '{print $2}'`}

   function wafls() {
   	ls $WAF_PATH/$1 ${${*}#${1}}
   }

   function wafcd() {
   	cd $WAF_PATH/$1 ${${*}#${1}}
   }

   function wafed() {
   	local PARAM=${${*}#${1}}
   	if [ -n `_editor_check "subl"` ]; then
   		subl $WAF_PATH/$1 $PARAM
   	elif [ -n `_editor_check "gedit"` ]; then
   		gedit $WAF_PATH/$1 $PARAM
   	elif [ -n `_editor_check "nano"` ]; then
   		nano $WAF_PATH/$1 $PARAM
   	elif [ -n `_editor_check "vi"` ]; then
   		vi $WAF_PATH/$1 $PARAM
   	fi
   }

   function wafbuild() {
   	local pwd_tmp=$PWD
   	cd $WAF_PATH &&	./waf && cd $pwd_tmp
   }

   function wafrun() {
   	local pwd_tmp=$PWD
   	cd $WAF_PATH &&	waf --run $* && cd $pwd_tmp
   }
   ```

   `source` the configuration file

4. `wafbuild && wafrun main` to run the simulation

### Todo

- [ ] CLICK router (?)
- [ ] Wi-Fi Collision