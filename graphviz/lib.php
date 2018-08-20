<?php

/** @file lib.php
 *  @brief Library Functions
 *  @author Richard Klein
 *  @date 2013 - 2014
 *
 */
require_once('config.php');
require_once('graph.php');

/**
 * @brief Fetch Moss index.html and cache it.
 * 
 * If the cache already exists just return the relevant HTML immediately.
 * 
 * @param int $mossID Moss submission ID.
 * @return string
 */
function fetchMoss($mossID) {
    global $config;

    $sContext = stream_context_create($config['proxy']);
    check_tmp($config['tmp']);

    $folder = $config['tmp'] . "/$mossID";
    $filename = $folder . '/index.html';
    if (!file_exists($filename)) {
        mkdir($folder);
        $source = file_get_contents(
                "http://moss.stanford.edu/results/$mossID/", // Moss result
                false, // Ignore include path search
                $sContext // Use the proxy
        );
        file_put_contents($filename, $source);
    } else {
        $source = file_get_contents($filename);
    }
    return $source;
}

/**
 * @brief Opens the Moss data and constructs the graph containing all edges.
 * 
 * @param type $mossID Moss ID number to search cache or download from Stanford.
 * @param type $lines
 * @return \graph
 */
function readMoss($mossID) {
    global $config;

    $source = fetchMoss($mossID);
    
    $doc = new DOMDocument();
    $doc->loadHTML($source);

    // Fetch html table
    $table = $doc->getElementsByTagName("table")->item(0);
    // Extract the rows from the table
    $rows = $table->getElementsByTagName("tr");
    // Construct graph
    $g = new graph();
    // For each row
    for ($i = 1; $i < $rows->length; $i++) {
        $row = $rows->item($i);
        $cols = $row->getElementsByTagName("td");

        $name1 = $cols->item(0)->nodeValue;
        $name2 = $cols->item(1)->textContent;
        $linem = $cols->item(2)->textContent;
        $link = $cols->item(0)->getElementsByTagName('a')->item(0)->getAttribute('href');

        $pattern = "|\./|";
        $name1 = trim(preg_replace($pattern, "", $name1));
        $name2 = trim(preg_replace($pattern, "", $name2));

        $pattern = "|.*\((..*)%\).*|";
        $p1 = trim(preg_replace($pattern, '\1', $name1));
        $p2 = trim(preg_replace($pattern, '\1', $name2));

        $pattern = "| \(..*%\)|";
        $name1 = trim(preg_replace($pattern, "", $name1));
        $name2 = trim(preg_replace($pattern, "", $name2));
        $linem = trim($linem);

        $g->addEdge($name1, $name2, $linem, $p1, $p2, $link);
    }

    return $g;
}

/**
 * @brief Generates both a PNG image and MAP of the graph.
 * 
 * The PNG is displayed and the MAP from graphviz allows one to click on the
 *  numbers in the graph to open each pairwise comparison.
 * @param type $result
 * @param type $lines
 */
function showMoss($linematches, $lines) {

    $g = $linematches->image("", $lines);
    $filename = $g['image_url'];

    echo "<div style='width: 100%; overflow-x: auto;'><img src=\"$filename\" USEMAP=\"#myGraph\"/></div><br/>\n";
    //echo file_get_contents($g['map_local']);
    //header("Content-type: image/png");
    //$image=imagecreatefromjpeg($_GET['img']);
    //imagejpeg($image);
    //readfile($filename);
}

/**
 * Check that the data location exists. If it doesn't, attempt to create it.
 * @param string $location Folder Location
 */
function check_tmp($location) {
    if (!file_exists($location)) {
        mkdir($location, 0777);
    }
}

/**
 * Reads most recent render parameters into \ref $config.
 * 
 * Parameters include:
 * -# moss_id
 * -# lines
 * -# no_lines
 * -# no_per
 * -# no_width
 */
function getParams() {
    global $config;

    if (isset($_GET['moss'])) {
        $params['moss'] = $_GET['moss'];
    }

    if (isset($_GET['lines'])) {
        $params['lines'] = $_GET['lines'];
    } else {
        $params['lines'] = 0;
    }

    if (isset($_GET['submitbutton'])) {
        $params['submitbutton'] = $_GET['submitbutton'];
    } else {
        $params['submitbutton'] = false;
    }

    if (isset($_GET['color'])) {
        $params['color'] = $_GET['color'];
    } else {
        $params['color'] = false;
    }

    if (isset($_GET['no_lines'])) {
        $params['no_lines'] = $_GET['no_lines'];
    } else {
        $params['no_lines'] = false;
    }

    if (isset($_GET['no_per'])) {
        $params['no_per'] = $_GET['no_per'];
    } else {
        $params['no_per'] = false;
    }

    if (isset($_GET['no_width'])) {
        $params['no_width'] = $_GET['no_width'];
    } else {
        $params['no_width'] = false;
    }

    $config['params'] = $params;
}

/**
 * Draw the render settings form.
 * 
 * Reads recent settings from \link config.php#$config $config['params'] \endlink
 * @param int $moss [in] Moss ID Number
 * 
 */
function drawForm($cmid) {
    global $config;
    getParams();

    $dot_file = file_get_contents('/var/www/html/moodle/temp.dot');

    $checked = '<option value="0">Disable</option>'.
                '<option value="1" selected="true">Enable</option>';

    $unchecked = '<option value="0" selected="true">Disable</option>'.
                 '<option value="1">Enable</option>';

    $cut_lines = $config['params']['lines'];

    $is_cofig_button = ($config['params']['submitbutton'] == "generate") ? true : false;

    $no_lines = $config['params']['no_lines'];
    if ($no_lines && $no_lines == 1 && $is_cofig_button) {
        $no_lines = $checked;
    } else {
        $no_lines = $unchecked;
    }

    $color = $config['params']['color'];
    if ($color && $color == 1 && $is_cofig_button) {
        $color = $checked;
    } else {
        $color = $unchecked;
    }

    $no_width = $config['params']['no_width'];
    if ($no_width && $no_width == 1 && $is_cofig_button) {
        $no_width = $checked;
    } else {
        $no_width = $unchecked;
    }

    $no_per = $config['params']['no_per'];
    if ($no_per && $no_per == 1 && $is_cofig_button) {
        $no_per = $checked;
    } else {
        $no_per = $unchecked;
    }
    $action = new moodle_url('/plagiarism/programming/view.php', array('cmid'=>$cmid, 'tool'=>'moss'));

    echo<<<FRM
    <script>

        //download.js v3.0, by dandavis; 2008-2014. [CCBY2] see http://danml.com/download.html for tests/usage
        // v1 landed a FF+Chrome compat way of downloading strings to local un-named files, upgraded to use a hidden frame and optional mime
        // v2 added named files via a[download], msSaveBlob, IE (10+) support, and window.URL support for larger+faster saves than dataURLs
        // v3 added dataURL and Blob Input, bind-toggle arity, and legacy dataURL fallback was improved with force-download mime and base64 support

        // data can be a string, Blob, File, or dataURL
         
                         
                         
        function download(data, strFileName, strMimeType) {
            
            var self = window, // this script is only for browsers anyway...
                u = "application/octet-stream", // this default mime also triggers iframe downloads
                m = strMimeType || u, 
                x = data,
                D = document,
                a = D.createElement("a"),
                z = function(a){return String(a);},
                
                
                B = self.Blob || self.MozBlob || self.WebKitBlob || z,
                BB = self.MSBlobBuilder || self.WebKitBlobBuilder || self.BlobBuilder,
                fn = strFileName || "download",
                blob, 
                b,
                ua,
                fr;

            //if(typeof B.bind === 'function' ){ B=B.bind(self); }
            
            if(String(this)==="true"){ //reverse arguments, allowing download.bind(true, "text/xml", "export.xml") to act as a callback
                x=[x, m];
                m=x[0];
                x=x[1]; 
            }
            
            
            
            //go ahead and download dataURLs right away
            if(String(x).match(/^data\:[\w+\-]+\/[\w+\-]+[,;]/)){
                return navigator.msSaveBlob ?  // IE10 can't do a[download], only Blobs:
                    navigator.msSaveBlob(d2b(x), fn) : 
                    saver(x) ; // everyone else can save dataURLs un-processed
            }//end if dataURL passed?
            
            try{
            
                blob = x instanceof B ? 
                    x : 
                    new B([x], {type: m}) ;
            }catch(y){
                if(BB){
                    b = new BB();
                    b.append([x]);
                    blob = b.getBlob(m); // the blob
                }
                
            }
            
            
            
            function d2b(u) {
                var p= u.split(/[:;,]/),
                t= p[1],
                dec= p[2] == "base64" ? atob : decodeURIComponent,
                bin= dec(p.pop()),
                mx= bin.length,
                i= 0,
                uia= new Uint8Array(mx);

                for(i;i<mx;++i) uia[i]= bin.charCodeAt(i);

                return new B([uia], {type: t});
             }
              
            function saver(url, winMode){
                
                
                if ('download' in a) { //html5 A[download]          
                    a.href = url;
                    a.setAttribute("download", fn);
                    a.innerHTML = "downloading...";
                    D.body.appendChild(a);
                    setTimeout(function() {
                        a.click();
                        D.body.removeChild(a);
                        if(winMode===true){setTimeout(function(){ self.URL.revokeObjectURL(a.href);}, 250 );}
                    }, 66);
                    return true;
                }
                
                //do iframe dataURL download (old ch+FF):
                var f = D.createElement("iframe");
                D.body.appendChild(f);
                if(!winMode){ // force a mime that will download:
                    url="data:"+url.replace(/^data:([\w\/\-\+]+)/, u);
                }
                 
            
                f.src = url;
                setTimeout(function(){ D.body.removeChild(f); }, 333);
                
            }//end saver 
                

            if (navigator.msSaveBlob) { // IE10+ : (has Blob, but not a[download] or URL)
                return navigator.msSaveBlob(blob, fn);
            }   
            
            if(self.URL){ // simple fast and modern way using Blob and URL:
                saver(self.URL.createObjectURL(blob), true);
            }else{
                // handle non-Blob()+non-URL browsers:
                if(typeof blob === "string" || blob.constructor===z ){
                    try{
                        return saver( "data:" +  m   + ";base64,"  +  self.btoa(blob)  ); 
                    }catch(y){
                        return saver( "data:" +  m   + "," + encodeURIComponent(blob)  ); 
                    }
                }
                
                // Blob but not URL:
                fr=new FileReader();
                fr.onload=function(e){
                    saver(this.result); 
                };
                fr.readAsDataURL(blob);
            }   
            return true;
        } /* end download() */


    </script>

    <form id="cs_parser" autocomplete="off" name="cs_parser" action="$action" class="mform" method="GET" style="margin-top: 20px; margin-bottom: 20px;">
        <fieldset class="clearfix collapsible" id="id_option_header">
            <legend class="ftoggler">Graph Config</legend>
            
            <input name="cmid" type="hidden" value="$cmid" />
            <input name="tool" type="hidden" value="moss" />

            <div class="fcontainer clearfix">
                <div id="fitem_id_cut_lines" class="fitem fitem_ftext ">
                    <div class="fitemtitle">
                        <label for="id_cut_lines">Cut Lines </label>
                        <span class="helptooltip">
                            <a href="http://52.32.199.111/moodle/help.php?component=plagiarism_programming&amp;identifier=rate_type_hlp&amp;lang=en" title="Help with " aria-haspopup="true" target="_blank">
                                <img src="http://52.32.199.111/moodle/theme/image.php/standard/core/1447052508/help" alt="Help with " class="iconhelp" />
                            </a>
                        </span>
                    </div>
                    <div class="felement ftext">
                        <input name="lines" type="text" value="$cut_lines" id="id_cut_lines" />
                    </div>
                </div>

                <div id="fitem_id_color" class="fitem fitem_ftext ">
                    <div class="fitemtitle">
                        <label for="id_color">Color </label>
                        <span class="helptooltip">
                            <a href="http://52.32.199.111/moodle/help.php?component=plagiarism_programming&amp;identifier=rate_type_hlp&amp;lang=en" title="Help with " aria-haspopup="true" target="_blank">
                                <img src="http://52.32.199.111/moodle/theme/image.php/standard/core/1447052508/help" alt="Help with " class="iconhelp" />
                            </a>
                        </span>
                    </div>
                    <div class="felement fselect">
                        <select name="color" id="id_color">$color</select>
                    </div>
                </div>

                <div id="fitem_id_lines_count" class="fitem fitem_fselect ">
                    <div class="fitemtitle">
                        <label for="id_lines_count">No Line Counts </label>
                        <span class="helptooltip">
                            <a href="http://52.32.199.111/moodle/help.php?component=plagiarism_programming&amp;identifier=rate_type_hlp&amp;lang=en" title="Help with " aria-haspopup="true" target="_blank">
                                <img src="http://52.32.199.111/moodle/theme/image.php/standard/core/1447052508/help" alt="Help with " class="iconhelp" />
                            </a>
                        </span>
                    </div>
                    <div class="felement fselect">
                        <select name="no_lines" id="id_lines_count">$no_lines</select>
                    </div>
                </div>

                <div id="fitem_id_line_thickness" class="fitem fitem_fselect ">
                    <div class="fitemtitle">
                        <label for="id_line_thickness">No Thickness </label>
                        <span class="helptooltip">
                            <a href="http://52.32.199.111/moodle/help.php?component=plagiarism_programming&amp;identifier=rate_type_hlp&amp;lang=en" title="Help with " aria-haspopup="true" target="_blank">
                                <img src="http://52.32.199.111/moodle/theme/image.php/standard/core/1447052508/help" alt="Help with " class="iconhelp" />
                            </a>
                        </span>
                    </div>
                    <div class="felement fselect">
                        <select name="no_width" id="id_line_thickness">
                            $no_width
                        </select>
                    </div>
                </div>

                <div id="fitem_id_line_percentage" class="fitem fitem_fselect ">
                    <div class="fitemtitle">
                        <label for="id_line_percentage">No Percentages </label>
                        <span class="helptooltip">
                            <a href="http://52.32.199.111/moodle/help.php?component=plagiarism_programming&amp;identifier=rate_type_hlp&amp;lang=en" title="Help with " aria-haspopup="true" target="_blank">
                                <img src="http://52.32.199.111/moodle/theme/image.php/standard/core/1447052508/help" alt="Help with " class="iconhelp" />
                            </a>
                        </span>
                    </div>
                    <div class="felement fselect">
                        <select name="no_per" id="id_line_percentage">
                            $no_per
                        </select>
                    </div>
                </div>

                <div id="fitem_id_submitbutton" class="fitem fitem_actionbuttons fitem_fsubmit">
                    <div class="felement fsubmit">
                        <input name="submitbutton" value="generate" type="submit" id="id_submitbutton" />
                    </div>
                </div>
                <div id="fitem_id_downloadbutton" class="fitem fitem_actionbuttons fitem_fsubmit">
                    <div class="felement fsubmit">
                        <input name="downloadbutton" value="Download" onclick="download_dot_file()" type="button" id="id_downloadbutton" />
                    </div>
                    <div id="id_dot_file" style="visibility: hidden; Display: none">
                        $dot_file
                    </div>
                    <script>
                        function download_dot_file() {
                            download(document.getElementById('id_dot_file').innerHTML, "similarity_report.dot", "text/plain");
                        }
                    </script>
                </div>
            </div>

        </fieldset>
    </form>
    
FRM;
}

/**
 * Display an error message.
 * @param string $message Error Text.
 * @todo Integrate with Bootstrap error box.
 */
function error_message($message) {
    echo $message . "<br />";
}
