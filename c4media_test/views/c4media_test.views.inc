<?php

/**
 * Implements hook_views_pre_render
 */
function c4media_test_views_pre_render(&$view) {
  switch ($view->name) {
    case 'presentations':
      foreach($view->result as &$row) {
	    if(empty($row->field_field_abstract) || empty($row->field_field_presentation_pdf)) {
		  $row->field_field_abstract_1[0]['rendered']['#markup'] = 'partial';
		}
		else {
		  $row->field_field_abstract_1[0]['rendered']['#markup'] = 'complete';
		}
	  }
    break;
  }
}