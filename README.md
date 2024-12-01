# safe_ptr_mgr
Safe pointer manager for c++ 17 and up. After adding thread safety to the class I tried to optimize the code as much as possible in an attempt to mitigate a slowdown I noticed through testing. I've done the best I can without resorting to some sort of lock-free pattern, which I have NO interest in in implementing.<br>I'd like to add a minimal suite of gtests just to show ME how safe the class actually is but I've got to fix some issues with my gtest installtion on this host first.

