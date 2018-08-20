<?php
global $CFG, $PAGE, $OUTPUT;
require_once(__DIR__.'/../../config.php');
require_once($CFG->libdir.'/adminlib.php');
require_once($CFG->libdir.'/plagiarismlib.php');
require_once($CFG->dirroot.'/plagiarism/programming/plagiarism_form.php');

require_login();
admin_externalpage_setup('plagiarismprogramming');

//$context = context_system::instance();
$context = get_context_instance(CONTEXT_SYSTEM);

require_capability('moodle/site:config', $context, $USER->id, true, "nopermissions");

require_once('plagiarism_form.php');
$mform = new plagiarism_setup_form();

$notification = '';
if ($mform->is_cancelled()) {
    redirect($CFG->wwwroot);
} else if (($data = $mform->get_data()) && confirm_sesskey()) {
    // update programming_use variable
    $programming_use = (isset($data->programming_use))?$data->programming_use:0;
    set_config('programming_use', $programming_use, 'plagiarism');

    $variables = array('level_enabled', 'moss_user_id', 'moss_user_id');

    $email = $data->moss_email;
    if ($email) {
        $pattern = '/\$userid=([0-9]+);/';
        preg_match($pattern, $email, $match);
        $data->moss_user_id = $match[1];
    }
    foreach ($variables as $field) {
        set_config($field, $data->$field, 'plagiarism_programming');
    }
    $notification = $OUTPUT->notification(get_string('save_config_success', 'plagiarism_programming'), 'notifysuccess');
}

$plagiarism_programming_setting = (array) get_config('plagiarism_programming');
$plagiarismsettings = (array) get_config('plagiarism');
if (isset($plagiarismsettings['programming_use'])) {
    $plagiarism_programming_setting['programming_use'] = $plagiarismsettings['programming_use'];
}

$mform->set_data($plagiarism_programming_setting);

echo $OUTPUT->header();

// include the javascript
$jsmodule = array(
    'name' => 'plagiarism_programming',
    'fullpath' => '/plagiarism/programming/coursesetting/course_selection.js',
    'requires' => array('panel', 'io'),
    'strings' => array(
        array('course_select', 'plagiarism_programming'),
        array('by_name', 'plagiarism_programming'),
        array('search', 'plagiarism_programming'),
        array('search_by_category', 'plagiarism_programming'),
    )
);
$PAGE->requires->js_init_call('M.plagiarism_programming.select_course.init', null, true, $jsmodule);

echo $OUTPUT->box_start('generalbox boxaligncenter', 'intro');
echo $notification;
$mform->display();
echo $OUTPUT->box_end();
echo $OUTPUT->footer();
?>