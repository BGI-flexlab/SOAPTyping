# SOAPtyping Docmentation


----------

## Intruduction
SOAPtyping is a novel HLA genotyping tool capable of producing accurate predictions from Sanger sequencing data files of both HLA classes I and II (Table1) by comparison to the IMGT/HLA database. 

>**Table 1** HLA molecules and the respective exon regions that can be analyzed by SOAPTyping
|Locus | exon|
| :--- | :--- |
|HLA-A	| 1,2,3,4,5
|HLA-B	|1,2,3,4,5
|HLA-C	|1,2,3,4,5,6,7
|HLA-DRB1	|1,2,3,4
|HLA-DRB3,4,5	|2,3
|HLA-DQA1	|1,2,3,4,
|HLA-DQB1	|2,3
|HLA-DPB1	|1,2,3,4

The Figure 1 is a snapshot of SOAPtyping. There are 6 function area:Toolbar, Base Navigator, Sequence display, Sample list display, Allele match list display and Electropherogram.


>**Figure 1** The SoapTyping application main window.
![mainUI][1]

The outline of the HLA typing workflow using SOAPTyping can be described as follow. 

 1. Sequences derived from the input files are aligned to the consensus sequence and alleles sequence in the database in order to select the eligible allele pairs.
 2. Then, the candidate allele pairs are sorted according the mismatches they contain and displayed in the allele match list (Fig 1).
 3. Finaly users should choose a suitable allele pair and edit the mismatched positions (if needed) until all the mismatch sites are eliminated to get the ultimate result. 

------

## Installation

----

## Propositional Workflow

The propositional workflow is demonstrated in Figure 2.
>**Figure 2** The propositional workflow of SOAPTyping 
![propositionalWorkflow][2]

The details are described below:

### Load AB1 files 
During the loading step, information of the imported files is shown in the ‘open file’ window (Fig 3). If the extracted file information is not true, users can modify them by choosing the correct information from the corresponding drop-down list. To utilize the automatic extraction function, the file name should include the sample name, the locus, the exon region combined with, the strand direction and the sequence id. These fields should be separated by an underscore and ordered correctly (e.g., RB16002353-A-70015-A01_A_2R_A02). For GSSP file, the field of the exon region combined with the strand direction（““R” in the example） should be replaced by GSSP name. After reassuring the file information, user can proceed to analysis. 
>**Figure 3** Loading input files 
![openFile][3]

### Select sequence files
After the loading step, imported sequence files are integrated based on sample name and locus and can be inspected in the sample list of the main window (Fig 1). If a sample is selected, sequence information, electropherogram and candidate allele pairs of this sample will be display in the corresponding region of the main window. 

### Check the mismatched positions
Two mismatch conditions, compatible mismatch and non-compatible mismatch, are considered in SOAPTyping. Compatible mismatch means that a site in the forward or pattern sequence (reverse or consensus sequence) includes the bases of the corresponding site in the reverse or consensus sequence (forward or pattern sequence), but they are different. For example, ‘R’ (heterozygous site of ‘A’ and ‘G’) and ‘A’ are compatible mismatch and ‘A’ and ‘G’ are non-compatible mismatch.
In this step, users need to first eliminate the non-compatible sites in forward and reverse sequence, and then eliminate the non-compatible sites in pattern sequence and consensus sequence. The mismatched sites are pointed out in the base navigator (Fig S1). User can skip to those positions by clicking on the indication bar and edit bases if needed after examining the electropherogram. To edit a base, click on the site on the electropherogram and input the correct base. Once a base is edited, SOAPTyping will analyze the sample again, update the allele match list and skip to next mismatched position automatically. If users want the analysis to be executed after all the mismatched bases are edited instead, choose ‘Edit Multi’ from the ‘Zoom’ list. 

### Examine the allele match list
By selecting a suitable allele pair in the allele match list, the sequences of the allele pair will be shown in the sequence display region. The mismatched sites of the pattern sequence and the allele pair sequences are highlighted in red in ‘Type Result’ and indicated in the lower area of the base navigator （Fig S1）. In this step, the combination of the allele pair sequences must be matched to the pattern sequence, the mismatch sites (non-compatible and compatible mismatch) should be eliminated.

### Save and import unfinished jobs
SOAPTyping offers two ways to save the results. One way is to save the individual sample by right-click on a sample in the sample list and choosing from “Quick Save”, “Quick Save and Clear” or “Quick Save by Date” options. The other way is clicking on the “Save” button in the toolbar (Fig S3) for saving multiple samples. During the saving process, the “Result” directory will be created in the work directory at the first time. In the “Result” directory each sample is assigned a subdirectory to save the original sequence files and the intermediate data that produced during the analysis. For subsequent analysis, users can import the saved files by clicking the ‘Load’ button (Fig S3), the software will search all the analysis files of the day and import them, the search scope also can be determined by date and the name of the file.

### GSSP prediction system
GSSP prediction system is used to resolve the ambiguity, which means there are multiple candidate allele pairs that have no mismatches. Before using a new GSSP, users need to add it to the database first, otherwise SOAPTyping cannot recognize it. In the allele match list, if an allele pair is marked as ‘Yes’ in the ‘GSSP’ column, it means that the software finds the suitable GSSP. To check GSSP information, right-click on the allele pair and choose ‘Show GSSP Z Code’, and then users need to re-sequence to get the GSSP files. After that, load the GSSP files and the corresponding sequence files that need to resolve the ambiguity, SOAPTyping will search the candidate allele pairs for each GSSP file, users need to choose the suitable allele pair and edit bases if mismatched sites exist. Finally, after all the GSSP files have been examined, click on ‘Combined’ under the sample will show the ultimate HLA typing result.

### Mark
Once the sequence files of a sample are imported, SOAPTyping will automatically mark the sample status based on the quality of the input files and the match condition of the pattern sequence and candidate allele pairs. An icon indicating the status (Fig S4) is shown before the sample name shown in the sample list. Users can also mark the sample as ‘Pending’, ‘Reviewed’ and ‘Approved’ in accordance with the analysis progress. When a defective sample comes up or analysis is not finished, it may be suitable to mark the sample as ‘Pending’ for subsequent analysis. If the typing result is reviewed, then it can be marked as ‘Reviewed’ and if the typing result is affirmed, the sample can be marked as ‘Approved’. The status also can be canceled by choosing ‘Unlock’. All the samples can be marked as ‘Reviewed’ or ‘Approved’ simultaneously by clicking on the corresponding button in the toolbar (Fig S3) while marking ‘Pending’ is only allowed for individual sample.

### Export reports
To export reports, click on the corresponding button in the toolbar (Fig S3) and the ‘Produce report’ window will appear. Users can determine the name of the report and the output directory. By checking ‘Ignore Indel’, alleles that contain indels will be excluded. The ‘Allele Count’ option is to control the number of the output alleles, otherwise all the alleles without mismatches will be exported by default. The report example is demonstrated in Figure S5.

### Database update
SOAPTyping offers the database update function to cater to the situation of frequent update of HLA alleles. Before updating the database, users need to go to the FTP directory of the IMGT database (ftp://ftp.ebi.ac.uk/pub/databases/ipd/imgt/hla/) to download the latest release of compressed alignments file (‘zip’ format, e.g., Alignments_Rel_3260.zip). After decompressing the downloaded zip file, users can get an ‘alignments’ folder which contains genomics sequence alignments, nucleotide sequence alignments and protein sequence alignments files for each locus, then select the nucleotide sequence alignments files and perform sequence format conversion using the scripts we provided on our website. This step generates several text files, including ‘geneTable’, ‘alleleTable’ and ‘labAlignTable’, these files are corresponding to ‘Gene File’, ‘Allele File’ and ‘Lab Ailgn File’ respectively in the database update window (Fig 4). 
>**Figure 4** Database update.
![databaseUpdate][4]


Additionally, if users want to update the GSSP information, text file contains the GSSP information should be prepared manually, examples are illustrated in Figure S6. Another way to update the GSSP information is filling the GSSP information in the corresponding fields in the ‘Insert GSSP’ window (Fig 5).
>**Figure 5** GSSP update. 
![gsspUpdate][5]

### Utilities
SOAPTyping supplies a set of utilities to assist the analysis. (1) Setting analysis threads. Click on the ‘Setting’ button (Fig S3) and choose ‘Set Thread’ to set a suitable number of threads to accelerate analysis. (2) Customizing the exon region for analysis. Click on the ‘Setting’ button and choose ‘Set Exon Trim’ to excluded the bases of both ends of the exon, excluded parts are not considered in analysis. (3) Alleles alignment. Click on the ‘Allele Alignment’ button (Fig S3) to invoke the alignment tool, users can choose a locus from the ‘Genes’ drop-down list and all the available alleles of this locus will be listed in the panel below. The alignment result will be displayed after the alleles of interest are chosen (Fig 6)

>**Figure 6** Allele alignment. 
![alleleAlignment][6]

  [1]: https://github.com/BGI-flexlab/SOAPtyping/tree/master/doc/images/mainUI.png
  [2]: https://github.com/BGI-flexlab/SOAPtyping/tree/master/doc/images/propositionalWorkflow.png
  [3]: https://github.com/BGI-flexlab/SOAPtyping/tree/master/doc/images/openFile.png
  [4]: https://github.com/BGI-flexlab/SOAPtyping/tree/master/doc/images/databaseUpdate.png
  [5]: https://github.com/BGI-flexlab/SOAPtyping/tree/master/doc/images/gsspUpdate.png
  [6]: https://github.com/BGI-flexlab/SOAPtyping/tree/master/doc/images/alleleAlignment.png