<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('C4Media Random Presentations'),
  'description' => t('Shows random presentations with basic configuration options.'),
  'category' => t('C4Media Test'),
  'edit form' => 'c4media_test_edit_form',
  'render callback' => 'c4media_test_render',
  'admin info' => 'c4media_test_admin_info',
  'defaults' => array(
    'presentations_to_show' => 3,
  )
);

/**
 * 'admin info' callback for panel pane.
 */
function c4media_test_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Showing @presentations_to_show random presentations.', array(
      '@presentations_to_show' => $conf['presentations_to_show'],
    ));
    return $block;
  }
}

/**
 * 'Edit form' callback for the content type.
 */
function c4media_test_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['presentations_to_show'] = array(
    '#title' => t('Presentations to show'),
    '#description' => t('The number of random presentations to show.'),
    '#type' => 'textfield',
    '#default_value' => $conf['presentations_to_show'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function c4media_test_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type)
 */
function c4media_test_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();

  $block->title = '';
  $block->content = '';

  $query = db_select('node', 'n');
  $alias = $query->join('users', 'u', 'u.uid = n.uid');
  $info = $query
			->fields('n', array('title'))
			->fields('u', array('name', 'picture'))
			->condition('n.type', 'presentation')
			->condition('u.picture', 0, '>')
			->range(0, $conf['presentations_to_show'])
			->orderRandom()
			->execute();

  $header = array(t('Presentation title'), t('Author name'), t('Author picture'));
  $rows = array();
  
  foreach($info as $inf) {
    $file = file_load($inf->picture);
	$picture = theme('image', array('path' => $file->uri, 'alt' => 'Alt text'));
    $rows[] = array(
	  $inf->title,
	  $inf->name,
	  $picture,
	);
  }
  $table = theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No presentations to show')));
  $block->content = $table;
  
  return $block;
}
