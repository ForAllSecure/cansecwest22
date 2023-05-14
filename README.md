# Reproducing Zero-Days with Mayhem

## Overview

From cell phones to DoD weapons systems, today, software is in most of the products that we use everyday. Hence, finding zero-days, vulnerabilities in software that no one but a hacker knows about, is a lucrative prospect. For a moment in time, when a hacker finds a new zero-day vulnerability, they potentially have the ability to attack potentially thousands of victims. Without a Dynamic Application Security Testing (DAST) tool in your development pipeline that can automate the discovery of new vulnerabilities, your application could have the next high profile zero day vulnerability. In this course, we’ll show you how to reproduce high-profile vulnerabilities using ForAllSecure’s Mayhem.

## Useful Links

* [Mayhem](https://mayhem.forallsecure.com)
* [Mayhem Hacking Community](https://community.forallsecure.com/)
* [TBONE Writeup](https://kunnamon.io/tbone/tbone-v1.0-redacted.pdf)
* [Mayhem Heroes Resources](https://github.com/mayhemheroes/hackathon-resources)

## Lab Links

* [Lab 1a. Your First Run](lighttpd-example.md#lab-1a-your-first-run)
* [Lab 1b. Mayhem CLI](lighttpd-example.md#lab-1b-run-with-the-mayhem-cli)
* [Lab 2. Reproducing Defects Locally](lighttpd-reproduce.md#lab-2-reproducing-defects-locally)
* [Lab 3. libbw64](libbw64-lab.md)
* [Lab 4. Harnessing Cereal](cereal-lab.md#part-1-harnessing-cereal)
* [Lab 5. U-Boot](uboot-lab.md)
* [Lab 6. Cereal Revisited](cereal-lab.md#part-2-improving-our-fuzzer)
* [Lab 7. Connman DNS Proxy](connman-patch-lab.md)
* [Lab 8. Reproducing Advanced Triage Defects](lighttpd-valgrind.md)
* [Lab 9. Connman DHCP Client](connman-syscall-lab.md)
