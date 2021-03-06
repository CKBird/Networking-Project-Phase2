# Ad Hoc Network Simulator with Collision Detection and Avoidance

## Authors: Christopher Bird and Blake Tacklind
#### Finish Date: 10th March, 2015
#### Date of Recent Major Update: 10th March, 2015

----

**Purpose**: Design and Simulate a flexible ad hoc network that has N hosts who all wish to send packets at negativly-distributed times based on a lambda and mu value. Each process signals separately, but there is only one channel between all hosts, and so only one packet can be sent at a time. Each sent packet must have an ACK packet sent back after the SIFS time of .05 msec. After the DIFS time of .1 msec, the channel is free again to have a packet sent. When a collision is detected, all attempeted senders generate a backoff time (that is different but on the same scale) and resend packet at the determined time.

----

Remaining Issues:

----

Change Log:
* 10th March: Turned it in
* 9th March : Finished it.
* 8th March : Code mostly written and bug tested. Not complete, current issues listed above
* 8th March : Legacy Code added for modification
* 8th March : Repository Made