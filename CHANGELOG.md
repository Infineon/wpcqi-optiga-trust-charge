## 1.2.1 (2023-11-06)

Bugfix:
  - Fixed parsing of hash value in `qi_auth_ptx_crypt_wocmd.c`

Maintenance:
  - Updated documentation (README, CHANGELOG, WPC Qi authentication protocol description)

## 1.2.0 (2022-08-19)

Maintenance:
  - Update of OPTIGA Host Lib submodule update

Bugfix:
  - Fixed APDU handling for responses: 
    - For a certificate chain a recommendation (see `qi_auth_ptx_only_test.c`) is to shift the buffer from there, otherwise an internal implementation would need more stack allocation.

## 1.1.0 (2021-08-10)

Features:
  - Added a new build configuration `optiga_ptx_without_cmd`,  with minimum COMMS layer (Infineon I2C Communication protocol).

## 1.0.0 (2021-02-10)

Initial release.
