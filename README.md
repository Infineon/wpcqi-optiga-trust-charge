# WPC Qi 1.3 Authentication protocol with</br> OPTIGA&trade; Trust Charge

## Description

This application note demonstrates partial implementation of the WPC Qi 1.3 Authentication Flow with help of the OPTIGA&trade; Trust Charge device authentication solution.
In particular this AppNote shows several things:
* API to form correct messages according to the WPC Qi 1.3. Authentication. It's important to highlight, that this code doesn't implement the authentication flow itself, rather just prepares messages which might be forwarded to the respective endpoint.
* Example usage of the API with a test flow
* Test Vectors against the API

Please check out also our description of the [WPC Qi 1.3 Authentication API](docs/WPC-Qi-Authentication-API.md) that shows you the essential message exchange and respective function calls.

## Hardware setup

For this application note, you need an official [OPTIGA&trade; Trust Charge Evaluation Kit](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-ch-eval-kit/)

<details> 
 <summary><em> OPTIGA&trade; Trust Charge Evaluation Kit </em></summary>
 <img src="https://www.infineon.com/export/sites/default/_images/product/security-smart-card-solutions/OPTIGA-Trust-Charge-Board.png_1681483313.png">
</details>

## Software setup

### Install DAVE IDE

NOTE: DAVE is supported on Windows platforms up to Windows 10.

Please follow these steps to install the DAVE IDE:

1. Go to Infineon's DAVE software download page.
2. Choose the DAVE package for your operating system and submit your registration information. After you register, you should receive a confirmation email with a link to download a .zip file.
3. Download the DAVE package .zip file (`DAVE_version_os_date.zip`), and unzip it to the location where you want to install DAVE (for example, `C:\DAVE4`).
   * NOTE: Some Windows users have reported problems using Windows Explorer to unzip the file. We recommend that you use a third-party program such as 7-Zip.
4. To launch DAVE, run the executable file found in the unzipped DAVE_version_os_date.zip folder.

After installing the DAVE IDE, please continue with the next section to install SEGGER JLink.

### Install SEGGER J-Link

To communicate with the XMC4700 Relax Kit's on-board debugging probe, you need the drivers included in the J-Link Software and Documentation pack. You can download the J-Link Software and Documentation pack from Segger's J-Link [software download page](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack).

### Additional Software References

For more information on DAVE IDE, see the [DAVE Quick Start Guide](https://www.infineon.com/dgdl/Infineon-DAVE_Quick_Start-GS-v02_00-EN.pdf) and visit [DAVE Forum](https://www.infineonforums.com/threads/6212-Install-DAVE%C2%99-IDE-for-XMC%C2%99-microcontrollers).

##  Setting up your hardware

To set up the OPTIGA&trade; Trust Charge Evaluation Kit, please follow these steps:

1. Connect your computer to the Debugger Micro USB port on your XMC4700 RelaxKit. The On-board USB debug probe is used to program the board.
2. For logging purpose please connect additional USB cable to the X100 Connector on the board (an opposite side of the board).

NOTE: Ensure that the Shield2Go Security OPTIGA™ Trust Charge in Socket 1 of the Infineon MyIoT Adapter

## Establishing a serial connection for debugging

The Demo project creates an USB HID device which can be opened in a tool like Terra Term.

## Downloading the Code

To get the code you need either download one of existing [releases](https://github.com/Infineon/wpcqi-optiga-trust-charge/releases) or Git clone this repository as below.

`git clone --recurse-submodules https://github.com/infineon/wpcqi-optiga-trust-charge`

## Importing the WPC Qi 1.3 Auth application note project into DAVE

1. Start DAVE.
2. In DAVE, choose File, and then choose Import. Expand the Infineon folder, choose DAVE Project, and then choose Next.
3. In the Import DAVE Projects window, choose Select Root Directory, choose Browse, and then choose the XMC4700 demo project.
4. In the directory where you have your cloned/downloaded sources the XMC4700 demo project is located in `projects/xmc4700_relax_kit_qi_auth_example/`
5. Make sure that Copy Projects Into Workspace is cleared.
6. Choose Finish.

The `xmc4700_qi_auth_example` project should be imported into your workspace and activated.

## Selecting the right build configuration

This example project tests the implementation of both sides PRx and PTx at the same time. However there is one detail, the Power Receiver can optionally do all crypto relevant functions either using OPTIGA or Third Party Crypto library as backend. You can choose it by selecting the corresponding configuration: Right Click on the Project and select "Build Configurations" -> "Set Active" -> Make a choice between the following configurations.

| Configuration            | PRx Crypto Backend         | PTx Crypto Backend                    | Description                                           |
| -------------------------|--------------------------- | ------------------------------------- | ----------------------------------------------------- |
| `optiga_prx_crypto`      | OPTIGA&trade; Trust Charge | OPTIGA&trade; Trust Charge            | Standard setup using OPTIGA&trade; Trust Charge for both PRx and PTx using the [OPTIGA&trade; Trust Charge Software Framework](https://github.com/Infineon/optiga-trust-charge) |
| `software_prx_crypto`    | mbedTLS software library   | OPTIGA&trade; Trust Charge            | PRx role: Using a purely software based approach using mbedTLS for the PRx side               |
| `optiga_ptx_without_cmd` | N/A                        | OPTIGA&trade; Trust Charge (minimal)  | PTx only minimalistic setup with minimum RAM and FLASH memory consumed and based on a pure ifx_i2c communication protocol implementation. In general the difference to the standard PTx implementation is [this file](https://github.com/Infineon/wpcqi-optiga-trust-charge/blob/master/wpc/PTx/qi_auth_ptx_crypt_wocmd.c). |

NOTE: The performance of the pure software based approach is significantly lower than the configuration based on the OPTIGA&trade; Trust Charge security controller.

In case the certificate and the corresponding private key are located in slots other than `0xE0E0`/`0xE0F0`, you can change this [here](https://github.com/Infineon/wpcqi-optiga-trust-charge/blob/15d8e7142f150832284054f6bb99414bcf832643/wpc/PTx/qi_auth_ptx_crypt.c#L46-L48)

## Output

![](https://github.com/Infineon/Assets/raw/master/Pictures/optiga_trust_charge_wpcqi.png)


## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
