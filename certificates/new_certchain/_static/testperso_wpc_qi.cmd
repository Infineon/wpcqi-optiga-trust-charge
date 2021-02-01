@echo off
set OPENSSL=%1
set CSR=%2
set STORAGE=..
)

REM Create Manfucating CA
call :genkey %STORAGE%\IFX_WPC_QI_13_Manufacturing_key
call :sign IFX_WPC_QI_13_Root_key.pem IFX_WPC_QI_13_CA_Config.cfg IFX_WPC_QI_13_Root_Certificate.pem 1234ABCD %STORAGE%\IFX_WPC_QI_13_Manufacturing_key.pem IFX_WPC_QI_13_Manufacturing_Request.conf IFX_WPC_QI_13_Manufacturing_Certificate

REM Create Product Unit Certificate from Trust Sculptor generated CSR
call :sign_csr %STORAGE%\IFX_WPC_QI_13_Manufacturing_key.pem IFX_WPC_QI_13_CA_Config.cfg %STORAGE%\IFX_WPC_QI_13_Manufacturing_Certificate.pem 1234EF01 %CSR% %STORAGE%\IFX_WPC_QI_13_ProductUnit_Certificate

exit /b


@rem SUBROUTINE
:genkey
set NAME=%1
REM   Generate Key (uncompressed)
%OPENSSL% ecparam -name prime256v1 -param_enc named_curve -genkey -noout -out %NAME%.pem
REM   Compress Key
%OPENSSL% ec -in %NAME%.pem -conv_form compressed -out %NAME%_comp.pem
del %NAME%.pem
move %NAME%_comp.pem %NAME%.pem
REM   Parse key
%OPENSSL% asn1parse -in %NAME%.pem  > %NAME%_asn1.txt
%OPENSSL% ec -in %NAME%.pem -outform DER -out %NAME%.der

exit /b

@rem SUBROUTINE
:sign

set CAKEY=%1
set CACFG=%2
set CACERT=%3
set SERIAL=%4
set CERTKEY=%5
set REQCFG=%6
set CERTNAME=%7

%OPENSSL% req -new -sha256 -key %CERTKEY% -config %REQCFG% -out %STORAGE%\%CERTNAME%.csr
%OPENSSL% req -in %STORAGE%\%CERTNAME%.csr -text > %STORAGE%\%CERTNAME%.csr.txt

@REM Sign CSR / Create cert
@del serial.txt*
@del certindex.txt*
@echo %SERIAL% > serial.txt
@copy NUL certindex.txt
@copy NUL certindex.txt.attr

REM
%OPENSSL% ca -keyfile %CAKEY% -batch -extensions %CERTNAME% -cert %CACERT% -config %CACFG%  -infiles %STORAGE%\%CERTNAME%.csr
@move %SERIAL%.pem %STORAGE%\%CERTNAME%.pem
@%OPENSSL% x509 -in %STORAGE%\%CERTNAME%.pem -outform DER -out %STORAGE%\%CERTNAME%.crt

@REM Crt to txt
@%OPENSSL% x509 -inform DER -in %STORAGE%\%CERTNAME%.crt -text > %STORAGE%\%CERTNAME%.txt

exit /b

@rem SUBROUTINE
:sign_csr
set CAKEY=%1
set CACFG=%2
set CACERT=%3
set SERIAL=%4
set CSR=%5
set CERTNAME=%6

REM pretty print for debugging
%OPENSSL% req -inform DER -in %CSR% -text > %CSR%.txt
REM OpenSSL seems to need CSRs in PEM format
%OPENSSL% req -inform DER -in %CSR% -out %CSR%.pem


@REM Sign CSR / Create cert
@del serial.txt*
@del certindex.txt*
@echo %SERIAL% > serial.txt
@copy NUL certindex.txt
@copy NUL certindex.txt.attr

REM
%OPENSSL% ca -keyfile %CAKEY% -batch -cert %CACERT% -config %CACFG% -infiles %CSR%.pem
@move %SERIAL%.pem %CERTNAME%.pem
@%OPENSSL% x509 -in %CERTNAME%.pem -outform DER -out %CERTNAME%.crt

@REM Crt to txt
@%OPENSSL% x509 -inform DER -in %CERTNAME%.crt -text > %CERTNAME%.txt

exit /b