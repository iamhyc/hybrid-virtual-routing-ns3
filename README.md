## NS3 Simulation

hybrid-virtual-routing, for VLC-RF Backend Wi-Fi Relay Network

### Howto


1. build ns3 following official course;

2. `ln -s <this-repo-full-path> <your-ns3-full-path>/examples/main`

3. add the following code in your `~\.bashrc` or `~\.zshrc`

   ```shell
   # Waf section
   WAF_PATH=/home/lab1112/workspace/bake/source/ns-3.27
   export PATH=$WAF_PATH:$PATH
   function wafrun() {
   	local pwd_tmp=$PWD
   	cd $WAF_PATH &&	waf --run=$1 &&	cd $pwd_tmp
   }
   ```

   `source` the configuration file

4. `wafrun main` to run the simulation

### Todo

- [ ] CLICK router (?)
- [ ] Wi-Fi Collision