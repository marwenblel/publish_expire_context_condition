<?php
/**
 * create a published date for a context
 */
class context_condition_publish extends context_condition {
    // Omit values since it could be any time
   function condition_values() {
       return array();
   }
   function condition_form($context) {
       $form = parent::condition_form($context);

       // Format the default value since it's saved as in the mktime() format
       if (!empty($form['#default_value'])) {
              $form['#default_value'] = array(
                'year' => date('Y', $form['#default_value'][0]),
                'month' => date('n', $form['#default_value'][0]),
                'day' => date('j', $form['#default_value'][0]),
              );
        } else {
                // Split up the current time
                $form['#default_value'] = array(
                'year' => date('Y', time()),
                'month' => date('n', time()),
                'day' => date('j', time()),
                );
}

                unset($form['#options']);
                $form['#type'] = 'date';
                return $form;

   
   }
    function condition_form_submit($values) {
            // Change format of date to timestamp
            return array(
                mktime(
                    0, // hour
                    0, // min
                    0, // sec
                    $values['month'], // month
                    $values['day'], // day
                    $values['year'] // year
                   ),
                 );


     }
   function execute ($value = NULL ) {
       // If nothing is passed then use now
       if (empty($value)) {
           $value = time();
       }

       foreach (context_enabled_contexts() as $context) {
           if ($date_time = $this->fetch_from_context($context, 'values')) {

               // If the current date is after the published date
               //then set the condition
               if ($date_time[0] <= $value) {
                   $this->condition_met($context, $value);
               }
           }
    }
   }

}