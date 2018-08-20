<?php
/** @file graph.php
 *  @brief Defines Graph Class (Adjacency Matrix, Directed, Weighted Graph)
 *  @author Richard Klein (University of the Witwatersrand, Johannesburg)
 *  @date 2013 - 2014
 *
 */

require_once('config.php');

/**
 * @brief Adjacency Matrix, Directed, Weighted Graph
 */
class graph{
    public $adj;	///< Adjacency Matrix
    public $gv;		///< Um?
    
    /**
     * @brief Default Constructor.
     * Constructor initialises #$adj as empty array.
     */
    public function graph(){
        $this->adj = array();
    }

    /**
     * @brief Add weighted, directed edge to the graph
     * @param type $name1 Source Vertex
     * @param type $name2 Destination Vertex
     * @param type $weight Lines Matching
     * @param type $p1 Percentage on Source
     * @param type $p2 Percentage on Destination
     * @param type $link HTTP URL of the pairwise comparison.
     */
    public function addEdge($name1, $name2, $weight, $p1, $p2, $link){
        if(!array_key_exists($name1, $this->adj)){
            $this->adj[$name1] = array();
        }
        if(!array_key_exists($name2, $this->adj)){
            $this->adj[$name2] = array();
        }

        $myEdge['w'] = $weight;
        $myEdge['l'] = $link;
        $myEdge['head'] = $p2;
        $myEdge['tail'] = $p1;
        
        $this->adj[$name1][$name2] = $myEdge;
        //$this->adj[$name2][$name1] = $myEdge;

    }

    /**
     * @brief Simple var_dump of the graph for debugging.
     */
    public function toString(){
        var_dump($this->adj);
    }
    /**
     * @brief Generate DOT graph description.
     * 
     * @param type $lines The least number of lines that must match.
     * @return string DOT description.
     */
    public function dot($lines){
        global $config;
        
        getParams();
        
        $text  = "graph { ";
        foreach ($this->adj as $s => $d) {
            foreach ($d as $v => $edge) {

                $w = $edge['head'];

                $l = $edge['l'];
                $head = $edge['head'];
                $tail = $edge['tail'];
                
                $no_width = $config['params']['no_width'];
                if($no_width){
                    $width = 1;
                }else{
                    $width = max($head, $tail)/20;
                }
                
                $no_lines = $config['params']['no_lines'];
                $no_per = $config['params']['no_per'];
                $color = $config['params']['color'];
                $cut_line = $config['params']['lines'];
                
                
                if($w >= $lines){
                    $score = $head;
                    $item_color = "black";
                    if ($score <= 30) {
                        $item_color = "blue";
                    } else if ($score > 30 && $score <= 50) {
                        $item_color = "green";
                    } else if ($score > 50 && $score <= 70) {
                        $item_color = "green";
                    } else if ($score > 70) {
                        $item_color = "red";
                    }

                    $per_ = '';
                    $lines_ = '';

                    if ($w <= $cut_line) {
                        $width = 0;
                    }
                    if (!$color) {
                        $item_color = "black"; 
                    } 
                    if(!$no_lines){
                        $lines_ = ",label=$w";
                    }
                    
                    if(!$no_per){
                        $per_ = ",headlabel=$head, taillabel=$tail";
                    }
                    if ($w <= $cut_line) {
                        $lines_ = '';
                        $per_ = '';
                    }
                    $text .= "\t\t" . $s . "--" . $v . "[color=$item_color,penwidth=$width" 
                    . $lines_ . $per_ . ", URL=\"$l\"];\n";
                }
            }
        }
        $text .= "}";

        return $text;
    }

    /**
     * Saves the generated PNG and MAP files.
     * @param int $moss_id Moss submission ID
     * @param int $lines Show only cases were lines matched > $lines
     * @return string[] {image_url, map_local} Image URL and Map filename.
     */
    public function image($moss_id, $lines ){
        global $config;

        $filename = "/var/www/html/moodle/temp";

        $text = $this->dot($lines);

        if (file_exists("$filename.map")) {
            unlink("$filename.map");
        }
        if (file_exists("$filename.png")) {
            unlink("$filename.png");
        }

        //file_put_contents("$filename.png", $text);

        // unflatten -f  -l 100 $filename.dot | /usr/bin/dot -Tcmapx -o $filename.map -Tpng -o $filename.png
        $a = exec('echo "'. $text .'" >> /var/www/html/moodle/temp.dot');
        $a = exec('echo "'. $text .'" | dot -Tpng >/var/www/html/moodle/temp.png');
        $a = exec('echo "'. $text .'" | dot -Timap >/var/www/html/moodle/temp.map');

        $out['image_url'] = "/moodle/temp.png"; 
        $out['dot_file'] = "/var/www/html/moodle/temp.dot"; 
        $out['map_local'] = "/var/www/html/moodle/temp.map";
        return $out;
    }
}


